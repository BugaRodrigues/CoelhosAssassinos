#include "Character/Camera/Camera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ACamera::ACamera()
{
	PrimaryActorTick.bCanEverTick = true;

	SmoothSpeed = 5.f;
	CameraOffset = FVector(-500.f, 0.f, 200.f);
	bHasTargetActor = false;

	Comp_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Comp_Camera);
	Comp_Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Comp_Camera->SetOrthoWidth(2048.f);
}

void ACamera::BeginPlay()
{
	Super::BeginPlay();
	
	if (!TargetActor) TargetActor = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	for (FParallaxSettings& Layer : ParallaxLayers)
	{
		if (Layer.LayerActor)
		{
			Layer.InitLocation = Layer.LayerActor->GetActorLocation();
		}
	}

	LastCameraPosition = GetActorLocation();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->SetViewTarget(this);
	}
}

void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor) return;

	FVector CurrentCameraPosition = GetActorLocation();
	FVector TargetPosition = TargetActor->GetActorLocation();

	TargetPosition.X = CurrentCameraPosition.X;

	float HalfWidth = Comp_Camera->OrthoWidth * 0.5f;
	float HalfHeight = (HalfWidth / GetViewportAspectRatio()) * 0.5f;

	TargetPosition.Y = FMath::Clamp(TargetPosition.Y, CameraBoundsMin.X + HalfWidth, CameraBoundsMax.X - HalfWidth);
	TargetPosition.Z = FMath::Clamp(TargetPosition.Z, CameraBoundsMin.Y + HalfHeight, CameraBoundsMax.Y - HalfHeight);

	if (!bHasTargetActor)
	{
		SetActorLocation(TargetPosition);
		LastCameraPosition = TargetPosition;
		bHasTargetActor = true;
		return;
	}

	FVector NewCameraPosition = FMath::VInterpTo(CurrentCameraPosition, TargetPosition, DeltaTime, SmoothSpeed);
	SetActorLocation(NewCameraPosition);

	UpdateParallax(NewCameraPosition - LastCameraPosition);
	LastCameraPosition = NewCameraPosition;
}

void ACamera::UpdateParallax(FVector CameraDelta)
{
	if (CameraDelta.IsNearlyZero()) return;

	for (FParallaxSettings& Layer : ParallaxLayers)
	{
		if (Layer.LayerActor)
		{
			FVector LayerCurrentLocation = Layer.LayerActor->GetActorLocation();
			float ParallaxEffect = 1.f - Layer.MovementMultiplier;

			FVector ParallaxMovement = LayerCurrentLocation + (CameraDelta * ParallaxEffect);
			ParallaxMovement.X = Layer.InitLocation.X;

			Layer.LayerActor->SetActorLocation(ParallaxMovement);
		}
	}
}

float ACamera::GetViewportAspectRatio() const
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		int32 ViewportX, ViewportY;

		PC->GetViewportSize(ViewportX, ViewportY);

		if (ViewportY != 0)
		{
			return static_cast<float>(ViewportX) / static_cast<float>(ViewportY);
		}
	}
	return 1.777778f;
}
