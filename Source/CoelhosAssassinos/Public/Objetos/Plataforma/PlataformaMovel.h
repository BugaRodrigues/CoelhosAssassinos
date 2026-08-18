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

			virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			void Esperar() { bCanMove = true; };

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			TObjectPtr<class UPaperFlipbookComponent> Comp_PaperFlipbook;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class USplineComponent> Comp_Spline;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float MovementSpeed;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Base;

			float CurrentDistance;

			int32 Direction;

			float SplineLenght;

			struct FTimerHandle TH_Movement;

			struct FTimerHandle TH_Esperar;

			bool bCanMove;

	#pragma endregion
	
};
