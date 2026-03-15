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

			void Move(const struct FInputActionValue& Value);

			void StartJump();

			void StopJump();

			void UpdateAnimation(EPlayerState State);

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

			bool bIsWalkingRight;

			float LastDirection;

	#pragma endregion
	
};
