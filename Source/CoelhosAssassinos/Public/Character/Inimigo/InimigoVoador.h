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

			float Direction;

			bool bIsSeeingPlayer;
			bool bCanSeePlayer;

			FVector PlayerLocation;

			FVector OriginalLocation;


	#pragma endregion
};
