#include "Character/Player/Cpp_Player.h"
#include "Character/Controller/Player/Player_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "PaperZDAnimationComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "PaperFlipbookComponent.h"
#include "InputActionValue.h"
#include "TimerManager.h"

ACpp_Player::ACpp_Player()
{
	PrimaryActorTick.bCanEverTick = false;

	bCanJump = true;

	SetRootComponent(GetCapsuleComponent());
	
	Comp_Animation = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("Animation Component"));

	Tags.Add(FName("Player"));
}

void ACpp_Player::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Pc = Cast<APlayer_Controller>(GetController());
	if (!Pc) return;

	Vida = Pc->GetVida();

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

	if (MenuAction)
	{
		EnhancedInput->BindAction(MenuAction, ETriggerEvent::Started, this, &ACpp_Player::OpenMenu);
	}
}

float ACpp_Player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Pc->PlaySound(SB_Lose);

	Vida--;

	LaunchCharacter(FVector(-50.f, 0.f, 750.f), true, true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Pc->AddOrRemoveImc(false);

	FName MapaName = FName(GetWorld()->GetName());
	float TempoDeQueda = ((-2.f * (750.f * 2)) / GetCharacterMovement()->GetGravityZ()) + 0.2f;
	
	TWeakObjectPtr<ACpp_Player> WeakThis(this);
	GetWorldTimerManager().SetTimer(TH_ReiniciarMapa, FTimerDelegate::CreateLambda([WeakThis] () 
	{
		if (!WeakThis.IsValid() ) return;

		if (!WeakThis->Pc && !WeakThis->SB_LoseAll) return;

		WeakThis->Pc->PlaySound(WeakThis->SB_LoseAll);
		WeakThis->Pc->ReiniciarMapa();
	}), TempoDeQueda, false);

	Pc->SaveGame(true);
	Pc->UpdateVida(Vida);

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

void ACpp_Player::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (Hit.GetActor() && !Pc)
	{
		bCanJump = true;

		if (!Hit.GetActor()->ActorHasTag("Inimigo")) 
		{ 
			Pc->PlaySound(SB_Land);
			return; 
		}

		Pc->PlaySound(SB_LandOnEnemy);

		LaunchCharacter(FVector(0.f, 0.f, 750.f), false, true);

		UGameplayStatics::ApplyDamage(Hit.GetActor(), 1, GetController(), this, UDamageType::StaticClass());
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
	if (!Pc && !bCanJump) return;

	bCanJump = false;

	Jump();

	Pc->PlaySound(SB_Jump);
}

void ACpp_Player::StopJump()
{
	StopJumping();
}

void ACpp_Player::OpenMenu()
{
	if (!Pc) return;

	Pc->AddOrRemoveMenuPause();
}

void ACpp_Player::CreateDynamicMaterial()
{
	if (!GetSprite()->GetMaterial(0)) return;

	UMaterialInterface* BaseMaterial = GetSprite()->GetMaterial(0);

	if (!BaseMaterial) return;

	Mat_Inst_Dynamic = GetSprite()->CreateDynamicMaterialInstance(0, BaseMaterial);
}
