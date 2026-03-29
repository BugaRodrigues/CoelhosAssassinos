#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chave.generated.h"

DECLARE_DELEGATE(FOnKeyCollected)

UCLASS()
class COELHOSASSASSINOS_API AChave : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			AChave();

			//virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	#pragma endregion

	#pragma region Variables

		public:	
			FOnKeyCollected OnKeyCollected;

		protected:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class UPaperSpriteComponent* Comp_Sprite;

	#pragma endregion
	
};
