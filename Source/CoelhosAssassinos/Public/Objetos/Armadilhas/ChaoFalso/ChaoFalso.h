#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaoFalso.generated.h"

UCLASS()
class COELHOSASSASSINOS_API AChaoFalso : public AActor
{
	GENERATED_BODY()

	#pragma region Functions

		public:	
			AChaoFalso();

			//virtual void Tick(float DeltaTime) override;

		protected:
			//virtual void BeginPlay() override;

			UFUNCTION()
			void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

			void Destruct();

			void Construct();

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class UPaperSpriteComponent* Comp_PaperSprite;

			UPROPERTY(EditAnywhere, category = "Sprites")
			TArray<class UPaperSprite*> Sprites;

			struct FTimerHandle TH_Destruct;

			struct FTimerHandle TH_Construct;

			bool bIsProcessing;

	#pragma endregion

};
