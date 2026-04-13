#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InimigoPulador.generated.h"

UCLASS()
class COELHOSASSASSINOS_API AInimigoPulador : public APaperCharacter
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			AInimigoPulador();

			//virtual void Tick(float DeltaTime) override;

			//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

			UFUNCTION()
			void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnParedeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnChaoEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

			void Jump();

		protected:
			virtual void BeginPlay() override;

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_ParedeCollision;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_ChaoCollision;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Velocidade;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float ForcaDoPulo;

			int32 Direction;

			struct FTimerHandle TH_Jump;

	#pragma endregion
	
};
