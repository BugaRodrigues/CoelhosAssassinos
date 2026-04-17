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
	bIsReturnig = false;

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnInimigoBeginOverlap);
	
	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Comp_BoxCollision->SetupAttachment(GetRootComponent());
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnSeePlayer);

	Tags.Add("Inimigo");
}

void AInimigoVoador::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	QueryParams.AddIgnoredActor(this);
	OriginalLocation = StartLocation = GetActorLocation();
	EndLocation = OriginalLocation + FVector(0.f, 0.f, -2000.f);

	SetSeeingPlayerCollision();

	GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
}

void AInimigoVoador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsSeeingPlayer && Player)
	{
		FVector PlayerLocation = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		AddMovementInput(PlayerLocation, 1.f);

		if (GetCharacterMovement()->MaxFlySpeed != 1200.f) GetCharacterMovement()->MaxFlySpeed = 1200.f;
	}
	else if (bIsReturnig) 
	{
		if (FMath::Abs(GetActorLocation().Z - OriginalLocation.Z) > 5.f)
		{
			AddMovementInput(FVector(0.f, 0.f, (OriginalLocation.Z > GetActorLocation().Z) ? 1.f : -1.f), 1.f);
		}
		else
		{
			bIsReturnig = false;
		}
	}
	else
	{
		if (GetCharacterMovement()->MaxFlySpeed != 600.f) GetCharacterMovement()->MaxFlySpeed = 600.f;

		AddMovementInput(FVector(0.f, Direction, 0.f), 1.f);

		float CurrentDistance = FVector::Distance(StartLocation, GetActorLocation());

		if (CurrentDistance >= DistanciaDeVoo && bDoOnce)
		{
			StartLocation = GetActorLocation();

			Direction *= -1;

			GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

			bDoOnce = false;

			GetWorldTimerManager().SetTimer(TH_DoOnce, this, &AInimigoVoador::DoOnce, .5f, false);
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
	if (OtherActor && OtherActor != this && !bIsFollowingPlayer)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		bIsSeeingPlayer = true;
		bIsFollowingPlayer = true;

		Player = OtherActor;

		GetWorldTimerManager().SetTimer(TH_DoOnce, this, &AInimigoVoador::StopSeePlayer, 2.f, false);
	}
}

void AInimigoVoador::SetSeeingPlayerCollision()
{
	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, OriginalLocation, EndLocation, ECC_WorldStatic, QueryParams);

	if (bHit)
	{
		if (!HitResult.GetActor()->ActorHasTag("Mapa"))
		{
			QueryParams.AddIgnoredActor(HitResult.GetActor());

			SetSeeingPlayerCollision();

			return;
		}

		FVector CurrentExtent = Comp_BoxCollision->GetUnscaledBoxExtent();

		Comp_BoxCollision->SetBoxExtent(FVector(CurrentExtent.X, CurrentExtent.Y, HitResult.Distance / 2));
		Comp_BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, -HitResult.Distance / 2));
	}
}
