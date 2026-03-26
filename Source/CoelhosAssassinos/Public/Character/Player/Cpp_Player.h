#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Cpp_Player.generated.h"

UENUM()
enum class EPlayerState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jumping UMETA(DisplayName = "Jmuping"),
	Falling UMETA(DisplayName = "Falling"),
	Death UMETA(DisplayName = "Death")
};

UCLASS()
class COELHOSASSASSINOS_API ACpp_Player : public APaperCharacter
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			ACpp_Player();

			//virtual void Tick(float DeltaTime) override;

			virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		protected:
			virtual void BeginPlay() override;

			virtual void Landed(const FHitResult& Hit) override;

			void Move(const struct FInputActionValue& Value);

			void StartJump();

			void StopJump();

			void UpdateAnimation(EPlayerState State);

			UFUNCTION()
			virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

			void TomouDano();

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class USpringArmComponent* Comp_SpringArm;

			UPROPERTY(EditAnywhere)
			class UCameraComponent* Comp_Camera;

			UPROPERTY(EditAnywhere, Category = "Mapping Input")
			TObjectPtr<class UInputMappingContext> MappingContext;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> MoveAction;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> JumpAction;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Vida;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Dano;

			UPROPERTY()
			class UMaterialInstanceDynamic* Mat_Inst_Dynamic;

			bool bIsWalkingRight;

			float LastDirection;

			struct FTimerHandle TH_TomouDano;

	#pragma endregion
	
};
