#include "Character/Player/Cpp_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Widgets/Contador/ContadorDeVida.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/Menu/Pause/MenuPause.h"
#include "Components/CapsuleComponent.h"
#include "PaperZDAnimationComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "TimerManager.h"

ACpp_Player::ACpp_Player()
{
	PrimaryActorTick.bCanEverTick = false;

	Dano = 10.f;
	bCanJump = true;

	SetRootComponent(GetCapsuleComponent());
	
	Comp_Animation = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("Animation Component"));

	Tags.Add(FName("Player"));
}

void ACpp_Player::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

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

	AddOrRemoveWidgetToViewport(true);
	AddMappingContext(true);
	CreateDynamicMaterial();
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
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!SaveGameInterface) { UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null")); return 0.f; }

	PlaySound(SB_Lose);

	Vida--;

	LaunchCharacter(FVector(-50.f, 0.f, 750.f), true, true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AddMappingContext(false);

	FName MapaName = FName(GetWorld()->GetName());
	float TempoDeQueda = ((-2.f * (750.f * 2)) / GetCharacterMovement()->GetGravityZ()) + 0.2f;
	
	GetWorldTimerManager().SetTimer(TH_ReiniciarMapa, this, &ACpp_Player::ReiniciarMapa, TempoDeQueda, false);

	SaveData->Vida = Vida;
	SaveGameInterface->SaveGame(true, SaveData);

	ContadorVida->SetVida(Vida);

	if (IsValid(Mat_Inst_Dynamic))
	{
		Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor::Red);

		TWeakObjectPtr<ACpp_Player> WeakSelf(this);
		GetWorldTimerManager().SetTimer(TH_TomouDano, FTimerDelegate::CreateLambda([WeakSelf]()
		{
			if (ACpp_Player* StrongSelf = WeakSelf.Get())
			{
				if (IsValid(StrongSelf->Mat_Inst_Dynamic))
				{
					StrongSelf->Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor::White);
				}
			}
		}), 0.1f, false);
	}

	return DamageAmount;
}

void ACpp_Player::ReiniciarMapa()
{
	if (SaveData->Vida <= 0)
	{
		PlaySound(SB_LoseAll);

		AddOrRemoveWidgetToViewport(false);

		SaveData = NewObject<UGame_Save>();
		if (GameInstance)
		{
			SaveGameInterface->SaveGame(true, SaveData);

			TWeakObjectPtr<ACpp_Player> WeakThis(this);
			GameInstance->OnSaveGameComplete.BindLambda([WeakThis]
			{
				if (!WeakThis.IsValid()) return;

				WeakThis->AddMenuPauseToViewport();
			});
		}
		else
		{
			SaveGameInterface->SaveGame(false, SaveData);

			AddMenuPauseToViewport();
		}
	}
	else
	{
		PlaySound(SB_Restart);

		UGameplayStatics::OpenLevel(this, FName(GetWorld()->GetName()));
	}
}

void ACpp_Player::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (Hit.GetActor())
	{
		bCanJump = true;

		if (!Hit.GetActor()->ActorHasTag("Inimigo")) 
		{ 
			PlaySound(SB_Land);
			return; 
		}

		PlaySound(SB_LandOnEnemy);

		LaunchCharacter(FVector(0.f, 0.f, 750.f), false, true);

		UGameplayStatics::ApplyDamage(Hit.GetActor(), Dano, GetController(), this, UDamageType::StaticClass());
	}
}

void ACpp_Player::Move(const FInputActionValue& Value)
{
	float MoveForce = Value.Get<float>();

	AddMovementInput(FVector(0.f, 1.f, 0.f), MoveForce);

	GetSprite()->SetRelativeRotation(FRotator(0.f, MoveForce > 0.f ? 0.f : 180.f, 0.f));
}

void ACpp_Player::StartJump()
{
	if (!bCanJump) return;

	bCanJump = false;

	Jump();

	PlaySound(SB_Jump);
}

void ACpp_Player::StopJump()
{
	StopJumping();
}

void ACpp_Player::AddMappingContext(bool AddOrRemove)
{
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem) return;

	if (!MappingContext) return;

	AddOrRemove ? Subsystem->AddMappingContext(MappingContext, 0) : Subsystem->RemoveMappingContext(MappingContext);
}

void ACpp_Player::CreateDynamicMaterial()
{
	if (!GetSprite()->GetMaterial(0)) return;

	UMaterialInterface* BaseMaterial = GetSprite()->GetMaterial(0);

	if (!BaseMaterial) return;

	Mat_Inst_Dynamic = GetSprite()->CreateDynamicMaterialInstance(0, BaseMaterial);
}

void ACpp_Player::AddOrRemoveWidgetToViewport(bool bAdd)
{
	if (bAdd)
	{
		if (!ContadorVidaClass) return;

		ContadorVida = CreateWidget<UContadorDeVida>(GetWorld(), ContadorVidaClass);

		if (!ContadorVida) return;

		ContadorVida->AddToViewport();
		ContadorVida->SetVida(Vida);
	}
	else
	{
		ContadorVida->RemoveFromParent();
	}
}

void ACpp_Player::AddMenuPauseToViewport()
{
	if (!MenuPauseClass) return;

	MenuPause = CreateWidget<UMenuPause>(GetWorld(), MenuPauseClass);

	if (!MenuPause) return;

	MenuPause->AddToViewport();
}

void ACpp_Player::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
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
