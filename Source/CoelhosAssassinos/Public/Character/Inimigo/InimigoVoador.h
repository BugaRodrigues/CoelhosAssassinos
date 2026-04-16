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

			void SetSeeingPlayerCollision();

			void DoOnce() { bDoOnce = true; };

			void StopSeePlayer() { bIsSeeingPlayer = false; bIsFollowingPlayer = false; Player = nullptr; bIsReturnig = true; };

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

			AActor* Player;

			bool bIsSeeingPlayer;
			bool bIsFollowingPlayer;
			bool bCanSeePlayer;
			bool bDoOnce;
			bool bIsReturnig;

			FVector OriginalLocation;
			FVector EndLocation;
			FVector StartLocation;

			struct FTimerHandle TH_DoOnce;
			struct FTimerHandle TH_StopSeePlayer;

			struct FCollisionQueryParams QueryParams;

	#pragma endregion
};
