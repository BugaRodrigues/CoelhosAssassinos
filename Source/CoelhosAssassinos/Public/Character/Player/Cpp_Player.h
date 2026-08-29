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

			void AddMappingContext(bool AddOrRemove);

			void CreateDynamicMaterial();

			void AddOrRemoveWidgetToViewport(bool bAdd);

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

			void ReiniciarMapa();

			void AddMenuPauseToViewport();

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variaveis

		public:	

		protected:
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

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UContadorDeVida> ContadorVidaClass;

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UMenuPause> MenuPauseClass;

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
			TObjectPtr<class UMaterialInstanceDynamic> Mat_Inst_Dynamic;

			UPROPERTY()
			TObjectPtr<class UGame_Save> GameData;

			UPROPERTY()
			TObjectPtr<class UContadorDeVida> ContadorVida;

			UPROPERTY()
			TObjectPtr<class UMenuPause> MenuPause;

			bool bCanJump;

			struct FTimerHandle TH_TomouDano;
			struct FTimerHandle TH_ReiniciarMapa;

			TObjectPtr<class IInterface_SaveGame> SaveGameInterface;

			TObjectPtr<class UMain_Game_Instance> GameInstance;

	#pragma endregion
	
};
