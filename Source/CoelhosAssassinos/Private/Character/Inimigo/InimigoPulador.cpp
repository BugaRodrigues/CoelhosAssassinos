#include "Character/Inimigo/InimigoPulador.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AInimigoPulador::AInimigoPulador()
{
	PrimaryActorTick.bCanEverTick = false;

	Velocidade = 300.f;
	ForcaDoPulo = 250.f;
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

	GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

	Comp_ChaoCollision->SetRelativeLocation(FVector(ForcaDoPulo, 0.f + 10.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10.f));

	GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, FMath::RandRange(.5f, 1.5f), true, 1.f);
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
		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void AInimigoPulador::Jump()
{ 
	if (!VerificarSeTemChao()) Direction *= -1; GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

	if (Target == ETargetJump::Player)
	{
		AInimigoPulador::LaunchCharacter(FVector(0.f, 0.f, ForcaDoPulo), false, true);
	}
	else
	{
		AInimigoPulador::LaunchCharacter(FVector(0.f, Direction * ForcaDoPulo, ForcaDoPulo), true, true);
	}
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
