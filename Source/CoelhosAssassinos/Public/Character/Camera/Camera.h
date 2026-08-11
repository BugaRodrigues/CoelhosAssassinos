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

	#pragma endregion

	#pragma region Variables

		public:
			UPROPERTY(EditAnywhere, Category = "Camera Setting")
			class UCameraComponent* Comp_Camera;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Follow")
			AActor* TargetActor;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Bounds")
			class ATriggerBox* CameraBounds;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Settings")
			float SmoothSpeed;

			UPROPERTY(EditAnywhere, Category = "Camera Setting | Settings")
			float VerticalViewHeight;

		protected:
		

	#pragma endregion

};
