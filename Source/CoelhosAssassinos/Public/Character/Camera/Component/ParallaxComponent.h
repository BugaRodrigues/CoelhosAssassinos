#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParallaxComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COELHOSASSASSINOS_API UParallaxComponent : public UActorComponent
{
	GENERATED_BODY()
	
	#pragma region Functions
		public:
			UParallaxComponent();

			virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		protected:
			virtual void BeginPlay() override;

	#pragma endregion
	
	#pragma region Properties
		public:
			UPROPERTY(EditAnywhere, Category="Parallax")
			FVector2D ParallaxFactor;

		protected:
			AActor* Camera;

			FVector LastCameraPosition;

	#pragma endregion
};
