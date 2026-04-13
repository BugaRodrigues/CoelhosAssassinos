#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InimigoVoador.generated.h"

UCLASS()
class COELHOSASSASSINOS_API AInimigoVoador : public APaperCharacter
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			AInimigoVoador();

			virtual void Tick(float DeltaTime) override;

			//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnInimigoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnSeePlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnEndSeePlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

			void SetSeeingPlayerCollision();

			void DoOnce() { bDoOnce = true; };

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Velocidade;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float DistanciaDeVoo;

			int32 Direction;

			bool bIsSeeingPlayer;
			bool bCanSeePlayer;
			bool bDoOnce;

			FVector PlayerLocation;

			FVector OriginalLocation;

			struct FTimerHandle TH_DoOnce;


	#pragma endregion
};
