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
	Landing UMETA(DisplayName = "Landing")
};

USTRUCT()
struct FPlayerAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* Run;

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* Jump;

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* Falling;

};

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

			virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

			void Move(const struct FInputActionValue& Value);

			void StartJump();

			void StopJump();

			void UpdateAnimation(EPlayerState State);

			UFUNCTION()
			virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

			void AddMappingContext();

			void CreateDynamicMaterial();

			void AddWidgetToViewport();

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class USpringArmComponent* Comp_SpringArm;

			UPROPERTY(EditAnywhere)
			class UCameraComponent* Comp_Camera;	

			UPROPERTY(EditAnywhere, Category = "Animations")
			struct FPlayerAnimations Animations;

			UPROPERTY(EditAnywhere, Category = "Mapping Input")
			TObjectPtr<class UInputMappingContext> MappingContext;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> MoveAction;

			UPROPERTY(EditAnywhere, Category = "Input Action")
			TObjectPtr<class UInputAction> JumpAction;

			UPROPERTY(EditAnywhere, Category = "Stats")
			int32 Vida;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Dano;

			UPROPERTY()
			class UMaterialInstanceDynamic* Mat_Inst_Dynamic;

			bool bIsWalkingRight;

			float LastDirection;

			struct FTimerHandle TH_TomouDano;

			UPROPERTY()
			class UGame_Save* SaveData;

			class IInterface_SaveGame* SaveGameInterface;

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UContadorDeVida> ContadorVidaClass;

			UPROPERTY()
			class UContadorDeVida* ContadorVida;

			class UMain_Game_Instance* GameInstance;

	#pragma endregion
	
};
