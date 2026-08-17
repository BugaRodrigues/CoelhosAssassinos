#include "Character/Camera/Component/ParallaxComponent.h"
#include "Kismet/GameplayStatics.h"

UParallaxComponent::UParallaxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ParallaxFactor = FVector2D(0.2f, 0.1f);
	ParallaxSpeed = 10.0f;
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

	FVector NewLocation = GetOwner()->GetActorLocation();
	NewLocation.Y += (DeltaTime * ParallaxFactor.X) * ParallaxSpeed;

	GetOwner()->SetActorLocation(NewLocation);
}

