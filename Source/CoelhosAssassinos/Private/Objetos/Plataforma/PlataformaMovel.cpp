#include "Objetos/Plataforma/PlataformaMovel.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h" 
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

APlataformaMovel::APlataformaMovel()
{
	PrimaryActorTick.bCanEverTick = false;

	CurrentDistance = 0.f;
	MovementSpeed = 5.f;
	Direction = 1;

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
	
	UpdateMovement();
}

void APlataformaMovel::UpdateMovement()
{
	if (!Comp_Spline) return;

	float SplineLenght = Comp_Spline->GetSplineLength();

	CurrentDistance += (MovementSpeed * .01f) * Direction;

	if (CurrentDistance >= SplineLenght)
	{
		CurrentDistance = SplineLenght;
		Direction = -1;

		GetWorld()->GetTimerManager().ClearTimer(TH_Movement);
		GetWorld()->GetTimerManager().SetTimer(TH_Esperar, this, &APlataformaMovel::Esperar, 1.f);
	}
	else if (CurrentDistance <= 0.f)
	{
		CurrentDistance = 0.f;
		Direction = 1;

		GetWorld()->GetTimerManager().ClearTimer(TH_Movement);
		GetWorld()->GetTimerManager().SetTimer(TH_Esperar, this, &APlataformaMovel::Esperar, 1.f);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TH_Movement, this, &APlataformaMovel::UpdateMovement, .01f);
	}

	Comp_BoxCollision->SetWorldLocation(Comp_Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
}

void APlataformaMovel::Esperar()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_Esperar);

	UpdateMovement();
}
