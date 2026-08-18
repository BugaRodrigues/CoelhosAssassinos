#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "CoelhoZumbi.generated.h"

UCLASS()
class COELHOSASSASSINOS_API ACoelhoZumbi : public APaperCharacter
{
	GENERATED_BODY()

	#pragma endregion Functions

		public:
			ACoelhoZumbi();

			virtual void Tick(float DeltaTime) override;

			//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnEndCollisionChao(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

			UFUNCTION()
			void OnBeginCollisionParede(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnInimigoBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			void Morreu();

			UFUNCTION()
			virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	#pragma endregion

	#pragma region Variaveis

		public:	

		private:
			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_Box_Chao;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_Box_Parede;

			UPROPERTY(EditAnywhere, category = "Stats")
			float Vida;

			UPROPERTY(EditAnywhere, category = "Stats")
			float Velocidade;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Run;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Dano;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Die;

			UPROPERTY()
			TObjectPtr<class UMaterialInstanceDynamic> Mat_Inst_Dynamic;

			float Direction;

			bool bCanMove;

			struct FTimerHandle TH_Morte;

	#pragma endregion

};
