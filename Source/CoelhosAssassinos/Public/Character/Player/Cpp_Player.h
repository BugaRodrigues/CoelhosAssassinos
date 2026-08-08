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
			TObjectPtr<class UPaperZDAnimationComponent> Comp_Animation;

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

			UPROPERTY()
			class UGame_Save* SaveData;

			UPROPERTY()
			class UContadorDeVida* ContadorVida;

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UContadorDeVida> ContadorVidaClass;

			struct FTimerHandle TH_TomouDano;

			class IInterface_SaveGame* SaveGameInterface;

			class UMain_Game_Instance* GameInstance;

	#pragma endregion
	
};
