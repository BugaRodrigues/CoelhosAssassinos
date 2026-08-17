#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

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

			void CalculateCameraBounds(float DeltaTime);

			//float UpdateCameraResolution();

	#pragma endregion

	#pragma region Variables

		public:
			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			TObjectPtr<class UCameraComponent> Comp_Camera;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Follow")
			TObjectPtr<AActor> TargetActor;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Bounds")
			TObjectPtr<AActor> MapaBounds;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Settings")
			float SmoothSpeed;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Settings")
			float TargetAspectRatio;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Settings")
			float DefaultOrthoWidth;

		protected:
		

	#pragma endregion

};
