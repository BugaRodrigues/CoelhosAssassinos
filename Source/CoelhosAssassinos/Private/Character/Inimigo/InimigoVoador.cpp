#include "Character/Inimigo/InimigoVoador.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"

AInimigoVoador::AInimigoVoador()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsSeeingPlayer = false;
	Velocidade = 300.f;
	DistanciaDeVoo = 250.f;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnInimigoBeginOverlap);
	
	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Comp_BoxCollision->SetupAttachment(GetRootComponent());
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnSeePlayer);
	Comp_BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInimigoVoador::OnEndSeePlayer);
}


void AInimigoVoador::BeginPlay()
{
	Super::BeginPlay();
	
	GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

	OriginalLocation = GetActorLocation();
}


void AInimigoVoador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSeeingPlayer)
	{
		AddMovementInput(PlayerLocation, 2.f);
	}
	else
	{
		AddMovementInput(FVector(0.f, Direction, 0.f), 1.f);

		if (FVector::Distance(GetActorLocation(), OriginalLocation) >= DistanciaDeVoo || FVector::Distance(GetActorLocation(), OriginalLocation) <= -DistanciaDeVoo)
		{
			Direction *= -1;

			GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
		}
	}
}

void AInimigoVoador::OnInimigoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void AInimigoVoador::OnSeePlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		bIsSeeingPlayer = true;

		PlayerLocation = OtherActor->GetActorLocation();
	}
}

void AInimigoVoador::OnEndSeePlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		bIsSeeingPlayer = false;
	}
}
