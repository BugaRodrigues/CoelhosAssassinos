#include "Objetos/Plataforma/PlataformaMovel.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h" 
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

APlataformaMovel::APlataformaMovel()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentDistance = 0.f;
	MovementSpeed = 5.f;
	Direction = 1;
	bCanMove = true;

	Comp_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	SetRootComponent(Comp_Spline);

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	Comp_BoxCollision->SetupAttachment(RootComponent);

	Comp_PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Paper Sprite Component"));
	Comp_PaperFlipbook->SetupAttachment(Comp_BoxCollision);
}

void APlataformaMovel::BeginPlay()
{
	Super::BeginPlay();

	if (!Comp_Spline) return;

	SplineLenght = Comp_Spline->GetSplineLength();
}

void APlataformaMovel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Comp_Spline || !bCanMove) return;

	CurrentDistance += (MovementSpeed * DeltaTime) * Direction;

	if (CurrentDistance >= SplineLenght)
	{
		CurrentDistance = SplineLenght;
		Direction = -1;
		bCanMove = false;

		GetWorld()->GetTimerManager().SetTimer(TH_Esperar, this, &APlataformaMovel::Esperar, 1.f);
	}
	else if (CurrentDistance <= 0.f)
	{
		CurrentDistance = 0.f;
		Direction = 1;
		bCanMove = false;

		GetWorld()->GetTimerManager().SetTimer(TH_Esperar, this, &APlataformaMovel::Esperar, 1.f);
	}

	Comp_BoxCollision->SetWorldLocation(Comp_Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
}
