#include "Character/Inimigo/InimigoVoador.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "PaperZDAnimationComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AInimigoVoador::AInimigoVoador()
{
	PrimaryActorTick.bCanEverTick = true;

	PatrolDistance = 500.f;
	PatrolSpeed = 600.f;
	ChaseSpeed = 1200.f;
	bIsFollowingPlayer = false;

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnInimigoBeginOverlap);
	
	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Comp_BoxCollision->SetupAttachment(GetRootComponent());
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoVoador::OnSeePlayer);

	Comp_Animation = CreateDefaultSubobject<UPaperZDAnimationComponent>(TEXT("PaperZD Animation Component"));

	Comp_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Componente Audio"));
	Comp_Audio->SetupAttachment(RootComponent);
	Comp_Audio->bAutoActivate = false;
	Comp_Audio->bOverrideAttenuation = true;

	Tags.Add("Inimigo");
}

void AInimigoVoador::BeginPlay()
{
	Super::BeginPlay();

	Direction = GetRootComponent()->GetRelativeRotation().Yaw > 0 ? 1 : -1;

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	QueryParams.AddIgnoredActor(this);
	OriginalLocation = GetActorLocation();

	SetSeeingPlayerCollision();
}

void AInimigoVoador::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (EnemyState)
	{
		case EEnemyState::Patrol:
			
			HandlePatrol(DeltaTime);

			break;
		case EEnemyState::Returning:
			
			HandleReturning(DeltaTime);

			break;
		case EEnemyState::Chase:

			HandleChase(DeltaTime);

			break;
		default:
			break;
	}
}

void AInimigoVoador::HandlePatrol(float DeltaTime)
{
	AddMovementInput(GetActorForwardVector(), PatrolSpeed * DeltaTime);

	if ((GetActorLocation() - OriginalLocation).SizeSquared() >= FMath::Square(PatrolDistance))
	{
		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

		EnemyState = EEnemyState::Returning;
	}
}

void AInimigoVoador::HandleChase(float DeltaTime)
{
	if (!Player || !bIsFollowingPlayer) 
	{
		Player = nullptr;
		bIsFollowingPlayer = false;
		EnemyState = EEnemyState::Returning;

		return;
	}

	FVector DirectionToPlayer = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();

	AddMovementInput(DirectionToPlayer, ChaseSpeed * DeltaTime);

	SetActorRotation(FRotator(0.f, DirectionToPlayer.Rotation().Yaw, 0.f));

	if ((Player->GetActorLocation() - GetActorLocation()).SizeSquared() <= FMath::Square(50.f))
	{
		Player = nullptr;
		bIsFollowingPlayer = false;
		EnemyState = EEnemyState::Returning;
	}
}

void AInimigoVoador::HandleReturning(float DeltaTime)
{
	PlaySound(SB_Returning);

	FVector DirectionToOriginal = (OriginalLocation - GetActorLocation()).GetSafeNormal();

	AddMovementInput(DirectionToOriginal, ChaseSpeed * DeltaTime);

	SetActorRotation(FRotator(0.f, DirectionToOriginal.Rotation().Yaw, 0.f));

	if ((GetActorLocation() - OriginalLocation).SizeSquared() <= FMath::Square(5.f))
	{
		SetActorLocation(OriginalLocation);

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

		EnemyState = EEnemyState::Patrol;
	}
}

void AInimigoVoador::OnInimigoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		if (OtherActor->GetActorLocation().Z < GetActorLocation().Z + (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2))
		{
			UGameplayStatics::ApplyDamage(OtherActor, 1.f, GetController(), this, UDamageType::StaticClass());

			EnemyState = EEnemyState::Returning;
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
}

void AInimigoVoador::OnSeePlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !bIsFollowingPlayer)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		PlaySound(SB_SawPlayer);

		bIsFollowingPlayer = true;

		Player = OtherActor;

		EnemyState = EEnemyState::Chase;

		GetWorldTimerManager().SetTimer(TH_StopSeePlayer, [this]() { EnemyState = EEnemyState::Returning; bIsFollowingPlayer = false; }, 2.f, false);
	}
}

void AInimigoVoador::SetSeeingPlayerCollision()
{
	FHitResult HitResult;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, OriginalLocation, OriginalLocation + FVector(0.f, 0.f, -2500.f), ECC_WorldStatic, QueryParams);

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

void AInimigoVoador::PlaySound(USoundBase* Sound)
{
	if (!Sound || !Comp_Audio) return;

	Comp_Audio->SetSound(Sound);
	Comp_Audio->Play();
};
