#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Cpp_Player.generated.h"

UCLASS()
class COELHOSASSASSINOS_API ACpp_Player : public APaperCharacter
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			ACpp_Player();

			virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		protected:
			virtual void BeginPlay() override;

			virtual void Landed(const FHitResult& Hit) override;

			//virtual void Tick(float DeltaTime) override;

			void Move(const struct FInputActionValue& Value);

			void StartJump();

			void StopJump();

			void PauseMenu();

			UFUNCTION()
			virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

			void CreateDynamicMaterial();

			void ReiniciarMapa();

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere, Category = "Animations")
			TObjectPtr<class UPaperZDAnimationComponent> Comp_Animation;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> MoveAction;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> JumpAction;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> MenuAction;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Jump;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Land;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_LandOnEnemy;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Lose;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_LoseAll;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Restart;

			UPROPERTY()
			int32 Vida;

			UPROPERTY()
			class APlayer_Controller* Pc;

			UPROPERTY()
			TObjectPtr<class UMaterialInstanceDynamic> Mat_Inst_Dynamic;

			bool bCanJump;

			struct FTimerHandle TH_TomouDano;
			struct FTimerHandle TH_ReiniciarMapa;

	#pragma endregion
	
};
