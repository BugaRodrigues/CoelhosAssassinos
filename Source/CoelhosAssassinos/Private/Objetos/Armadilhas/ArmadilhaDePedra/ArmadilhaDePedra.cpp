#include "Objetos/Armadilhas/ArmadilhaDePedra/ArmadilhaDePedra.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AArmadilhaDePedra::AArmadilhaDePedra()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentState = EFallingState::Idle;
	FallingSpeed = 2500.f;
	ReturnSpeed = 10.f;
	MaxFallDistance = 5000.f;
	Damage = 20.f;

	Comp_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Comp_Scene);

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Comp_BoxCollision->SetupAttachment(RootComponent);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AArmadilhaDePedra::OnBeginOverlap);

	Comp_PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	Comp_PaperSprite->SetupAttachment(Comp_BoxCollision); 
	Comp_PaperSprite->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	Comp_BoxProximadade = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxProximadade"));
	Comp_BoxProximadade->SetupAttachment(RootComponent);
	Comp_BoxProximadade->OnComponentBeginOverlap.AddDynamic(this, &AArmadilhaDePedra::OnPlayerPassou);
}

void AArmadilhaDePedra::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	EndLocation = StartLocation + FVector(0.f, 0.f, -1500.f);
	QueryParams.AddIgnoredActor(this);

	SetProximidade();
}

void AArmadilhaDePedra::OnPlayerPassou(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor && OtherActor == this) return;

	if (CurrentState != EFallingState::Idle) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	CurrentState = EFallingState::Falling;

	GetWorld()->GetTimerManager().SetTimer(TH_Falling, this, &AArmadilhaDePedra::StartFalling, 0.01f, true);
}

void AArmadilhaDePedra::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor && OtherActor == this) return;

	if (OtherActor->ActorHasTag(FName("Player")))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());

		GetWorld()->GetTimerManager().ClearTimer(TH_Falling);

		GetWorld()->GetTimerManager().SetTimer(TH_Returning, this, &AArmadilhaDePedra::StartReturning, 0.01f, true);
	}
	else if (OtherActor->ActorHasTag(FName("Mapa")))
	{
		CurrentState = EFallingState::Returning;

		GetWorld()->GetTimerManager().ClearTimer(TH_Falling);

		GetWorld()->GetTimerManager().SetTimer(TH_Returning, this, &AArmadilhaDePedra::StartReturning, 0.01f, true, 1.f);
	}
}

void AArmadilhaDePedra::SetProximidade()
{
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_WorldStatic, QueryParams);

	if (bHit)
	{
		FVector CurrentExtent = Comp_BoxProximadade->GetUnscaledBoxExtent();

		Comp_BoxProximadade->SetBoxExtent(FVector(CurrentExtent.X, CurrentExtent.Y, HitResult.Distance / 2));
		Comp_BoxProximadade->SetRelativeLocation(FVector(0.f, 0.f, -HitResult.Distance / 2));
	}
}

void AArmadilhaDePedra::StartFalling()
{
	FVector NewLocation = Comp_BoxCollision->GetComponentLocation();

	NewLocation.Z -= FallingSpeed * .01f;

	Comp_BoxCollision->SetWorldLocation(NewLocation);

	if (FVector::Dist(StartLocation, NewLocation) >= MaxFallDistance)
	{
		CurrentState = EFallingState::Returning;

		GetWorld()->GetTimerManager().ClearTimer(TH_Falling);

		GetWorld()->GetTimerManager().SetTimer(TH_Returning, this, &AArmadilhaDePedra::StartReturning, 0.01f, true);

		return;
	}
}

void AArmadilhaDePedra::StartReturning()
{
	FVector CurrentLocation = Comp_BoxCollision->GetComponentLocation();

	FVector ReturnLocation = FMath::InterpEaseOut(CurrentLocation, StartLocation, .01f, ReturnSpeed);

	Comp_BoxCollision->SetWorldLocation(ReturnLocation);

	if (CurrentLocation.Equals(StartLocation, 1.f))
	{
		Comp_BoxCollision->SetWorldLocation(StartLocation);

		CurrentState = EFallingState::Idle;

		GetWorld()->GetTimerManager().ClearTimer(TH_Returning);

		return;
	}
}
