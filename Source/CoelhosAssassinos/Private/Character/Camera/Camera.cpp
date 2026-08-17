#include "Character/Camera/Camera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperTileMapComponent.h"
#include "Engine/TriggerBox.h"
#include "PaperTileMap.h"

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SmoothSpeed = 2.f;
	DefaultOrthoWidth = 1920.f;
	TargetAspectRatio = 1.777f;

	Comp_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Comp_Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Comp_Camera->OrthoWidth = DefaultOrthoWidth;
	Comp_Camera->AspectRatio = TargetAspectRatio;
	Comp_Camera->bConstrainAspectRatio = true;
	SetRootComponent(Comp_Camera);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();

	if (!TargetActor)
	{
		TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PC->SetViewTarget(this);
	}
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateCameraBounds(DeltaTime);
}

void ACamera::CalculateCameraBounds(float DeltaTime)
{
	if (!TargetActor || !MapaBounds) return;

	float HalfWidth = Comp_Camera->OrthoWidth * .5f;
	float HalfHeight = (Comp_Camera->OrthoWidth / Comp_Camera->AspectRatio) * .5f;

	FVector MapOrigin, MapExtent;
	MapaBounds->GetActorBounds(false, MapOrigin, MapExtent);

	FVector MapMin = MapOrigin - MapExtent;
	FVector MapMax = MapOrigin + MapExtent;

	FVector PlayerLocation = TargetActor->GetActorLocation();
	FVector DesiredLocation = PlayerLocation;

	if (HalfWidth * 2.f > MapExtent.Y * 2.f)
	{
		DesiredLocation.Y = MapOrigin.Y;
	}
	else
	{
		DesiredLocation.Y = FMath::Clamp(DesiredLocation.Y, MapMin.Y + HalfWidth, MapMax.Y - HalfWidth);
	}

	if (HalfHeight * 2.f > MapExtent.Z * 2.f)
	{
		DesiredLocation.Z = MapOrigin.Z;
	}
	else
	{
		DesiredLocation.Z = FMath::Clamp(DesiredLocation.Z, MapMin.Z + HalfHeight, MapMax.Z - HalfHeight);
	}

	DesiredLocation.X = GetActorLocation().X;

	SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaTime, SmoothSpeed));
}
