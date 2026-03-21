#include "Character/Player/Cpp_Player.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "TimerManager.h"

ACpp_Player::ACpp_Player()
{
	PrimaryActorTick.bCanEverTick = false;

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
}

void ACpp_Player::BeginPlay()
{
	Super::BeginPlay();

	#pragma region Add Mapping Context 
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (!PC) return;

		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (!Subsystem) return;

		if (MappingContext)
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	#pragma endregion

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
	Vida -= DamageAmount;

	GetSprite()->SetSpriteColor(FColor::Red);

	GetWorld()->GetTimerManager().SetTimer(TH_TomouDano, this, &ACpp_Player::TomouDano, .05f);

	/*
	if (Vida <= 0)
	{
		UKismetSystemLibrary::QuitGame();
	}
	*/

	return DamageAmount;
}

void ACpp_Player::Move(const FInputActionValue& Value)
{
	AddMovementInput(FVector(0.f, 1.f, 0.f), Value.Get<float>());

	bIsWalkingRight = Value.Get<float>() > 0;

	if (Value.Get<float>() != LastDirection || LastDirection == 0.f)
	{
		UpdateAnimation(Value.Get<float>() == 0.f ? EPlayerState::Idle : EPlayerState::Walking);
	}

	LastDirection = Value.Get<float>();
}

void ACpp_Player::StartJump()
{
	Jump();

	UpdateAnimation(EPlayerState::Jumping);
}

void ACpp_Player::StopJump()
{
	StopJumping();
}

void ACpp_Player::UpdateAnimation(EPlayerState State)
{
	switch (State)
	{
		case EPlayerState::Idle:
			return;
		case EPlayerState::Walking:

			GetSprite()->SetRelativeRotation(FRotator(0., bIsWalkingRight ? 0. : 180., 0. ));

			return;
		case EPlayerState::Jumping:
			return;
		case EPlayerState::Falling:
			return;
		case EPlayerState::Death:
			return;
		default:
			return;
	}
}

void ACpp_Player::TomouDano()
{
	GetSprite()->SetSpriteColor(FColor::White);
}
