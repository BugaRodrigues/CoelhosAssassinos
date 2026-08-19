#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InimigoVoador.generated.h"

UENUM()
enum class EEnemyState : uint8
{
	Patrol,
	Returning,
	Chase
};

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

			void HandlePatrol(float DeltaTime);

			void HandleReturning(float DeltaTime);

			void HandleChase(float DeltaTime);

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UAudioComponent> Comp_Audio;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UPaperZDAnimationComponent> Comp_Animation;

			UPROPERTY(EditAnywhere, Category = "State")
			FVector OriginalLocation;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float PatrolDistance;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float PatrolSpeed;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float ChaseSpeed;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Flying;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_SawPlayer;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Returning;

			EEnemyState EnemyState;

			int32 Direction;

			AActor* Player;

			bool bIsFollowingPlayer;

			struct FTimerHandle TH_StopSeePlayer;

			struct FCollisionQueryParams QueryParams;

	#pragma endregion
};
