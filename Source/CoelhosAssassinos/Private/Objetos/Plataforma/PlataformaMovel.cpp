#include "Objetos/Plataforma/PlataformaMovel.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h" 
#include "PaperSpriteComponent.h"

APlataformaMovel::APlataformaMovel()
{
	PrimaryActorTick.bCanEverTick = false;

	MovementSpeed = 300.f;
	CurrentDistance = 0.f;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Comp_BoxCollision);

	Comp_PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite Component"));
	Comp_PaperSprite->SetupAttachment(RootComponent);

	Comp_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	Comp_Spline->SetupAttachment(RootComponent);
}

void APlataformaMovel::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlataformaMovel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Comp_Spline) return;

	CurrentDistance += MovementSpeed * DeltaTime;

	if (CurrentDistance > Comp_Spline->GetSplineLength())
	{
		CurrentDistance = FMath::Fmod(CurrentDistance, Comp_Spline->GetSplineLength());
	}

	SetActorLocation(Comp_Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
}
