#include "Character/Inimigo/InimigoPulador.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

AInimigoPulador::AInimigoPulador()
{
	PrimaryActorTick.bCanEverTick = false;

	Velocidade = 300.f;
	ForcaDoPulo = 500.f;
	Direction = 1;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnBeginOverlap);

	Comp_ParedeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Parede"));
	Comp_ParedeCollision->SetupAttachment(GetRootComponent());
	Comp_ParedeCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnParedeBeginOverlap);

	Tags.Add("Inimigo");
}

void AInimigoPulador::BeginPlay()
{
	Super::BeginPlay();
	
	BoxExtent = FVector(10.f, 10.f, 10.f);
	BoxRotation = GetActorQuat();
	CollisionParams.AddIgnoredActor(this);
	CollisionShape = FCollisionShape::MakeBox(BoxExtent);

	GetSprite()->SetFlipbook(Animations.Jump ? Animations.Jump : nullptr);
	GetSprite()->SetLooping(false);
	GetSprite()->SetPlaybackPositionInFrames(0, false);
	GetSprite()->Stop();

	GetRootComponent()->SetRelativeRotation(SetDirectionRotation(Direction));

	GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, FMath::RandRange(.5f, 1.5f), true, 1.f);
}

void AInimigoPulador::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (GetCharacterMovement()->MovementMode == MOVE_Falling) UpdateAnimation(EEnemyAnimState::Falling);
}

void AInimigoPulador::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	UpdateAnimation(EEnemyAnimState::Landed);
}

void AInimigoPulador::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			Target = ETargetJump::Player;
		}
		else
		{
			Target = ETargetJump::Mapa;
		}
	}
}

void AInimigoPulador::OnParedeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		GetRootComponent()->SetRelativeRotation(SetDirectionRotation(Direction *= -1));
	}
}

void AInimigoPulador::Jump()
{ 
	if (!VerificarSeTemChao())
	{
		GetRootComponent()->SetRelativeRotation(SetDirectionRotation(Direction *= -1));
	}

	if (Target == ETargetJump::Player)
	{
		GetSprite()->SetRelativeRotation(SetDirectionRotation(Direction));
		AInimigoPulador::LaunchCharacter(FVector(0.f, 0.f, ForcaDoPulo), false, true);
	}
	else
	{
		AInimigoPulador::LaunchCharacter(FVector(0.f, Direction * ForcaDoPulo, ForcaDoPulo), true, true);
	}

	UpdateAnimation(EEnemyAnimState::Jumping);
}

bool AInimigoPulador::VerificarSeTemChao()
{
	StartLocation = GetActorLocation() + FVector(0.f, (ForcaDoPulo / 2) * Direction, -35.f);
	EndLocation = StartLocation + FVector(0.f, 10.f * Direction, -10.f);

	bool Hit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, BoxRotation, ECC_WorldStatic, CollisionShape, CollisionParams);

	if (Hit)
	{
		if (HitResult.GetActor()->ActorHasTag("Mapa")) return true;
	}
	return false;
}

FRotator AInimigoPulador::SetDirectionRotation(int32 NewDirection)
{
	Direction = (NewDirection >= 0) ? 1 : -1;
	float YawRotation = (Direction < 0) ? -90.f : 90.f;
	return FRotator(0.f, YawRotation, 0.f);
}

void AInimigoPulador::UpdateAnimation(EEnemyAnimState EnemyState)
{
	switch (EnemyState)
	{
		case EEnemyAnimState::Jumping:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Jumping"));

			/*if (GetSprite()->GetFlipbook() != Animations.Jump)
			{
				GetSprite()->SetFlipbook(Animations.Jump ? Animations.Jump : nullptr);
				GetSprite()->SetLooping(false);
			}
			GetSprite()->PlayFromStart();*/
			break;
		case EEnemyAnimState::Falling:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Falling"));

			/*if (GetSprite()->GetFlipbook() != Animations.Falling)
			{
				GetSprite()->SetFlipbook(Animations.Falling ? Animations.Falling : nullptr);
				GetSprite()->SetPlaybackPositionInFrames(0, false);
				GetSprite()->Stop();
			}*/
			break;
		case EEnemyAnimState::Landed:

			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Landed"));

			/*if (GetSprite()->GetFlipbook() != Animations.Falling)
			{
				GetSprite()->SetFlipbook(Animations.Falling ? Animations.Falling : nullptr);
				GetSprite()->SetPlaybackPositionInFrames(0, false);
				GetSprite()->Stop();
			}
			GetSprite()->PlayFromStart();*/
			break;
		default:
			break;
	}
}
