#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Espinhos.generated.h"

UCLASS()
class COELHOSASSASSINOS_API AEspinhos : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			AEspinhos();

		private:
			//virtual void BeginPlay() override;

			UFUNCTION()
			void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	#pragma endregion

	#pragma region Variaveis

		public:

		private:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class UPaperSpriteComponent* Comp_PaperSprite;

	#pragma endregion

};
