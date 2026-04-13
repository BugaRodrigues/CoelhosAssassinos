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
	bDoOnce = true;
	Velocidade = 300.f;
	DistanciaDeVoo = 250.f;
	Direction = 1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.f;

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnInimigoBeginOverlap);
	
	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Comp_BoxCollision->SetupAttachment(GetRootComponent());
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnSeePlayer);
	Comp_BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AInimigoVoador::OnEndSeePlayer);

	Tags.Add("Inimigo");
}


void AInimigoVoador::BeginPlay()
{
	Super::BeginPlay();
	
	SetSeeingPlayerCollision();

	GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
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

		if (FVector::Distance(OriginalLocation, GetActorLocation()) >= DistanciaDeVoo && bDoOnce)
		{
			Direction *= -1;

			GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

			bDoOnce = false;

			GetWorldTimerManager().SetTimer(TH_DoOnce, this, &AInimigoVoador::DoOnce, .05f, false);
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

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Colidiu com o Inimigo Voador");
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

void AInimigoVoador::SetSeeingPlayerCollision()
{
	OriginalLocation = GetActorLocation();
	FVector EndLocation = OriginalLocation + FVector(0.f, 0.f, -1500.f);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, OriginalLocation, EndLocation, ECC_WorldStatic, QueryParams);

	if (bHit)
	{
		FVector CurrentExtent = Comp_BoxCollision->GetUnscaledBoxExtent();

		Comp_BoxCollision->SetBoxExtent(FVector(CurrentExtent.X, CurrentExtent.Y, HitResult.Distance / 2));
		Comp_BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, -HitResult.Distance / 2));
	}
}
