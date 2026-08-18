#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Trampolim.generated.h"

UCLASS()
class COELHOSASSASSINOS_API ATrampolim : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			ATrampolim();

			//virtual void Tick(float DeltaTime) override;

		private:
			//virtual void BeginPlay() override;

			UFUNCTION()
			void OnJumpAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			void PlaySound();

	#pragma endregion

	#pragma region Variaveis

		public:

		private:
			UPROPERTY(EditAnywhere)
			class UPaperFlipbookComponent* Comp_PaperFlipbook;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_Box_JumpArea;

			UPROPERTY(EditAnywhere, Category = "Lauch")
			float LaunchForce;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Jumping;

	#pragma endregion

};
