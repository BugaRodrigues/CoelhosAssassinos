#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlataformaMovel.generated.h"

UCLASS()
class COELHOSASSASSINOS_API APlataformaMovel : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			APlataformaMovel();

			//virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			void UpdateMovement();

			void Esperar();

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UPaperFlipbookComponent* Comp_PaperFlipbook;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class USplineComponent* Comp_Spline;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float MovementSpeed;

		private:
			float CurrentDistance;

			int32 Direction;

			struct FTimerHandle TH_Movement;

			struct FTimerHandle TH_Esperar;

	#pragma endregion
	
};
