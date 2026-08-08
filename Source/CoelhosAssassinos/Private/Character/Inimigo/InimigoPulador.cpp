#include "Character/Inimigo/InimigoPulador.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "PaperZDAnimationComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

AInimigoPulador::AInimigoPulador()
{
	PrimaryActorTick.bCanEverTick = false;

	Velocidade = 150.f;
	AlturaDoPulo = 225.f;

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
		SetDirectionRotation(Direction *= -1);
	}
}

void AInimigoPulador::Jump()
{ 
	if (!VerificarSeTemChao()) SetDirectionRotation(Direction *= -1);

	float Gravidade = GetCharacterMovement()->GetGravityZ() * -1;
	float vz0 = FMath::Sqrt(2.f * Gravidade * AlturaDoPulo);
	TempoDoPulo = (2.f * vz0) / Gravidade;
	DistanciaDoPulo = Velocidade * TempoDoPulo;

	AInimigoPulador::LaunchCharacter(FVector(0.f, (Target == ETargetJump::Player) ? 0.f : DistanciaDoPulo * Direction, vz0), true, true);

	GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, FMath::RandRange(1.5f, 2.f));
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
