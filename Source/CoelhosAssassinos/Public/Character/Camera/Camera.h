#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

USTRUCT()
struct FParallaxSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	AActor* LayerActor;

	UPROPERTY(EditAnywhere)
	float MovementMultiplier = .5f;

	FVector InitLocation;
};

UCLASS()
class COELHOSASSASSINOS_API ACamera : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:
			ACamera();

			virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			void UpdateParallax(FVector CameraDelta);

	#pragma endregion

	#pragma region Variables

		public:
			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			AActor* TargetActor;

			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			FVector2D CameraBoundsMin;

			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			FVector2D CameraBoundsMax;

			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			float SmoothSpeed;

			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			FVector CameraOffset;

			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			TArray<FParallaxSettings> ParallaxLayers;

		protected:
			UPROPERTY(EditAnywhere)
			class UCameraComponent* Comp_Camera;

			FVector LastCameraPosition;

			bool bHasTargetActor;

			float GetViewportAspectRatio() const;

	#pragma endregion

};
