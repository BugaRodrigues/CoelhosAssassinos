#include "Character/Camera/Component/ParallaxComponent.h"
#include "Kismet/GameplayStatics.h"

UParallaxComponent::UParallaxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ParallaxFactor = FVector2D(0.2f, 0.1f);
}

void UParallaxComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetViewTarget();

	if (Camera)
	{
		LastCameraPosition = Camera->GetActorLocation();
	}
}

void UParallaxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!Camera) return;

	FVector CurrentCameraPosition = Camera->GetActorLocation();
	FVector CameraDelta = CurrentCameraPosition - LastCameraPosition;

	FVector NewLocation = GetOwner()->GetActorLocation();
	NewLocation.Y += CameraDelta.Y * ParallaxFactor.X;
	NewLocation.Z += CameraDelta.Z * ParallaxFactor.Y;

	GetOwner()->SetActorLocation(NewLocation);
	LastCameraPosition = CurrentCameraPosition;
}

