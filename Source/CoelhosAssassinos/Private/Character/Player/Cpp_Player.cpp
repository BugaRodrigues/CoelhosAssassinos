#include "Character/Player/Cpp_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Widgets/Contador/ContadorDeVida.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "TimerManager.h"

ACpp_Player::ACpp_Player()
{
	PrimaryActorTick.bCanEverTick = false;

	Dano = 10.f;

	SetRootComponent(GetCapsuleComponent());

	Comp_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	Comp_SpringArm->SetupAttachment(RootComponent);
	Comp_SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 50.f), FRotator(0.f, -90.f, 0.f));
	Comp_SpringArm->TargetArmLength = 700.f;
	Comp_SpringArm->bEnableCameraLag = true;
	Comp_SpringArm->CameraLagSpeed = 100.f;

	Comp_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Comp_Camera->SetupAttachment(Comp_SpringArm, USpringArmComponent::SocketName);
	Comp_Camera->bAutoCalculateOrthoPlanes = false;
	Comp_Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	
	Tags.Add(FName("Player"));
}

void ACpp_Player::BeginPlay()
{
	Super::BeginPlay();

	#pragma region SaveGame
		GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());

		 if (GameInstance)
		 {
			SaveGameInterface = Cast<IInterface_SaveGame>(GameInstance);
		 }
		 else
		 {
			 UE_LOG(LogTemp, Error, TEXT("GameInstance is null"));
		 }

		if (SaveGameInterface) 
		{ 
			SaveData = SaveGameInterface->GetSaveData();
			Vida = SaveData->Vida;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null"));
		}
	#pragma endregion

	AddWidgetToViewport();
	AddMappingContext();
	CreateDynamicMaterial();
	UpdateAnimation(EPlayerState::Idle);
}

void ACpp_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Nao Encontrado!!!"));
		return;
	}

	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACpp_Player::Move);
	}

	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACpp_Player::StartJump);

		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACpp_Player::StopJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ACpp_Player::StopJump);
	}
}

float ACpp_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!SaveGameInterface) { UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null")); return 0.f; }

	Vida--;

	if (Vida <= 0)
	{  
		SaveData = NewObject<UGame_Save>();

		if (IsValid(GameInstance))
		{
			SaveGameInterface->SaveGame(true, SaveData);

			UWorld* World = GetWorld();
			FName MapaName = SaveData->MapaName;

			GameInstance->OnSaveGameComplete.BindLambda([World, MapaName]()
			{
				UGameplayStatics::OpenLevel(World, MapaName);
			});
		}
		else
		{
			SaveGameInterface->SaveGame(false, SaveData);

			UGameplayStatics::OpenLevel(this, SaveData->MapaName);
		}

		return DamageAmount;
	}

	SaveData->Vida = Vida;

	SaveGameInterface->SaveGame(true, SaveData);

	ContadorVida->SetVida(Vida);

	if (IsValid(Mat_Inst_Dynamic))
	{
		Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor::Red);

		GetWorldTimerManager().SetTimer(TH_TomouDano, [this]() 
		{
			if (IsValid(Mat_Inst_Dynamic))
			{
				Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor::White);
			}
		}, 0.1f, false);
	}

	return DamageAmount;
}

void ACpp_Player::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (Hit.GetActor())
	{
		UpdateAnimation(EPlayerState::Landing);

		GetWorldTimerManager().ClearTimer(TH_TestFalling);

		if (!Hit.GetActor()->ActorHasTag("Inimigo")) return;

		LaunchCharacter(FVector(0.f, 0.f, 750.f), false, true);

		UGameplayStatics::ApplyDamage(Hit.GetActor(), Dano, GetController(), this, UDamageType::StaticClass());
	}
}

void ACpp_Player::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		if (GetVelocity().Z < 0.f)
		{
			GetWorldTimerManager().SetTimer(TH_TestFalling, [this]()
			{
				GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Blue, FString::Printf(TEXT("Z Velocity: %f"), GetVelocity().Z));
			}, .1f, true);

			UpdateAnimation(EPlayerState::Falling);
		}
	}
}

void ACpp_Player::Move(const FInputActionValue& Value)
{
	float MoveForce = Value.Get<float>();

	AddMovementInput(FVector(0.f, 1.f, 0.f), MoveForce);

	if (FMath::IsNearlyZero(MoveForce))
	{
		UpdateAnimation(EPlayerState::Idle);
	}
	else if (MoveForce != LastDirection || LastDirection == 0.f)
	{
		bIsWalkingRight = MoveForce > 0;

		UpdateAnimation(EPlayerState::Walking);
	}
	LastDirection = MoveForce;
}

void ACpp_Player::StartJump() { Jump(); UpdateAnimation(EPlayerState::Jumping); }

void ACpp_Player::StopJump() { StopJumping(); }

void ACpp_Player::UpdateAnimation(EPlayerState State)
{
	switch (State)
	{
		case EPlayerState::Idle:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Parado"));

			/*if (GetSprite()->GetFlipbook() != Animations.Run)
			{
				GetSprite()->SetFlipbook(Animations.Run ? Animations.Run : nullptr);
			}
			GetSprite()->SetPlaybackPositionInFrames(0, false);
			GetSprite()->Stop();*/
			return;
		case EPlayerState::Walking:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Andando para %s"), bIsWalkingRight ? TEXT("direita") : TEXT("esquerda")));

			/*if (GetSprite()->GetFlipbook() != Animations.Run)
			{
				GetSprite()->SetFlipbook(Animations.Run ? Animations.Run : nullptr);
			}
			GetSprite()->Play();*/
			GetSprite()->SetRelativeRotation(FRotator(0., bIsWalkingRight ? 0. : 180., 0. ));
			return;
		case EPlayerState::Jumping:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Pulo"));

			/*if (GetSprite()->GetFlipbook() != Animations.Jump)
			{
				GetSprite()->SetFlipbook(Animations.Jump ? Animations.Jump : nullptr);
			}*/
			return;
		case EPlayerState::Falling:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Caindo"));

			/*if (GetSprite()->GetFlipbook() != Animations.Falling)
			{
				GetSprite()->SetFlipbook(Animations.Falling ? Animations.Falling : nullptr);
			}
			GetSprite()->SetPlaybackPositionInFrames(0, false);
			GetSprite()->Stop();*/
			return;
		case EPlayerState::Landing:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, TEXT("Queda"));

			/*if (GetSprite()->GetFlipbook() != Animations.Falling)
			{
				GetSprite()->SetFlipbook(Animations.Falling ? Animations.Falling : nullptr);
			}
			GetSprite()->Play();*/
			return;
		default:
			return;
	}
}

void ACpp_Player::AddMappingContext()
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem) return;

	if (MappingContext)
	{
		Subsystem->AddMappingContext(MappingContext, 0);

	}
}

void ACpp_Player::CreateDynamicMaterial()
{
	if (!GetSprite()->GetMaterial(0)) return;

	UMaterialInterface* BaseMaterial = GetSprite()->GetMaterial(0);

	if (!BaseMaterial) return;

	Mat_Inst_Dynamic = GetSprite()->CreateDynamicMaterialInstance(0, BaseMaterial);
}

void ACpp_Player::AddWidgetToViewport()
{
	if (!ContadorVidaClass) return;

	ContadorVida = CreateWidget<UContadorDeVida>(GetWorld(), ContadorVidaClass);

	if (!ContadorVida) return;

	ContadorVida->AddToViewport();
	ContadorVida->SetVida(Vida);
}

void ACpp_Player::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::LevelTransition || EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (IsValid(GameInstance))
		{
			GameInstance->OnSaveGameComplete.Unbind();
		}
	}
}
