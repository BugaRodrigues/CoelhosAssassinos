#include "Character/Camera/Camera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TriggerBox.h"
#include "DrawDebugHelpers.h"

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SmoothSpeed = 2.f;
	VerticalViewHeight = 350.f;

	Comp_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Comp_Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
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
	if (!TargetActor || !GEngine->GameViewport || !CameraBounds) return;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	if (ViewportSize.X <= 0 || ViewportSize.Y <= 0) return;

	float AspectRatio = (ViewportSize.Y > 0) ? (ViewportSize.X / ViewportSize.Y) : 1.77f;
	Comp_Camera->OrthoWidth = VerticalViewHeight * AspectRatio;

	float HalfWidth = Comp_Camera->OrthoWidth * .5f;
	float HalfHeight = VerticalViewHeight * .5f;

	FVector BoundsOrigin;
	FVector BoundsExtent;
	CameraBounds->GetActorBounds(false, BoundsOrigin, BoundsExtent);

	float MinY = (BoundsOrigin.Y - BoundsExtent.Y) + HalfWidth;
	float MaxY = (BoundsOrigin.Y + BoundsExtent.Y) - HalfWidth;
	float MinZ = (BoundsOrigin.Z - BoundsExtent.Z) + HalfHeight;
	float MaxZ = (BoundsOrigin.Z + BoundsExtent.Z) - HalfHeight;

	FVector TargetLocation = TargetActor->GetActorLocation();

	float ClampedY = FMath::Clamp(TargetLocation.Y, MinY, MaxY);
	float ClampedZ = FMath::Clamp(TargetLocation.Z, MinZ, MaxZ);

	if (MaxY < MinY) ClampedY = BoundsOrigin.Y;
	if (MaxZ < MinZ) ClampedZ = BoundsOrigin.Z;

	FVector CurrentLocation = GetActorLocation();
	float SmoothY = FMath::FInterpTo(CurrentLocation.Y, ClampedY, DeltaTime, SmoothSpeed);
	float SmoothZ = FMath::FInterpTo(CurrentLocation.Z, ClampedZ, DeltaTime, SmoothSpeed);

	SetActorLocation(FVector(CurrentLocation.X, SmoothY, SmoothZ));
}
