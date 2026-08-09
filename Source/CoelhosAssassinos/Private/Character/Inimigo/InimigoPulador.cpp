#include "Character/Inimigo/InimigoPulador.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "PaperZDAnimationComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AInimigoPulador::AInimigoPulador()
{
	PrimaryActorTick.bCanEverTick = false;

	Velocidade = 150.f;
	AlturaDoPulo = 225.f;

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnBeginOverlap);

	Comp_ParedeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Parede"));
	Comp_ParedeCollision->SetupAttachment(GetRootComponent());
	Comp_ParedeCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnParedeBeginOverlap);

	Comp_Animation = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("Animation Component"));

	Tags.Add("Inimigo");
}

void AInimigoPulador::BeginPlay()
{
	Super::BeginPlay();
	
	Direction = GetRootComponent()->GetRelativeRotation().Yaw > 0 ? 1 : -1;
	BoxExtent = FVector(10.f, 10.f, 10.f);
	BoxRotation = GetActorQuat();
	CollisionParams.AddIgnoredActor(this);
	CollisionShape = FCollisionShape::MakeBox(BoxExtent);

	SetDirectionRotation(Direction);

	GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, FMath::RandRange(2.f, 3.5f), false, 1.f);
}

void AInimigoPulador::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			if (OtherActor->GetActorLocation().Z < GetActorLocation().Z + (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2))
			{
				Target = ETargetJump::Player;

				UGameplayStatics::ApplyDamage(OtherActor, 1.f, GetController(), this, UDamageType::StaticClass());

				GetWorldTimerManager().ClearTimer(TH_Jump);
				GetCharacterMovement()->StopMovementImmediately();
				Jump();
			}
			else
			{
				GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

				GetWorldTimerManager().SetTimerForNextTick([this]()
				{
					GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
				});
			}
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
		SetDirectionRotation(Direction *= -1);
	}
}

void AInimigoPulador::Jump()
{ 
	if (!VerificarSeTemChao()) SetDirectionRotation(Direction *= -1);

	float Gravidade = GetCharacterMovement()->GetGravityZ() * -1;
	float Vz0 = FMath::Sqrt(2.f * Gravidade * AlturaDoPulo);
	TempoDoPulo = (2.f * Vz0) / Gravidade;
	DistanciaDoPulo = Velocidade * TempoDoPulo;

	if (Target == ETargetJump::Player)
	{
		LaunchCharacter(FVector(0.f, 0.f, Vz0), false, true);
	}
	else
	{
		LaunchCharacter(FVector(0.f, DistanciaDoPulo * Direction, Vz0), true, true);
	}

	GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, FMath::RandRange(1.5f, 3.5f));
}

bool AInimigoPulador::VerificarSeTemChao()
{
	StartLocation = GetActorLocation() + FVector(0.f, DistanciaDoPulo * TempoDoPulo * Direction, -40.f);
	EndLocation = StartLocation + FVector(0.f, 10.f * Direction, -10.f);

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, StartLocation, EndLocation, BoxRotation, ECC_WorldStatic, CollisionShape, CollisionParams);

	if (bHit)
	{
		return (HitResult.GetActor()->ActorHasTag("Mapa")) ? true : false;
	}
	return false;
}

void AInimigoPulador::SetDirectionRotation(int32 NewDirection)
{
	Direction = NewDirection;
	float YawRotation = (Direction < 0) ? -90.f : 90.f;

	GetRootComponent()->SetRelativeRotation(FRotator(0.f, YawRotation, 0.f));
}
