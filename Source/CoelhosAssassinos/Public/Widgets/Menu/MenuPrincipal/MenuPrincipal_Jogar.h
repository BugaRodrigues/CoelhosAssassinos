#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPrincipal_Jogar.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UMenuPrincipal_Jogar : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions

		public:

		protected:
			virtual void NativeConstruct() override;
		
			virtual void NativeDestruct() override;

			UFUNCTION()
			void OnClicked_BTN_NewGame();

			UFUNCTION()
			void OnClicked_BTN_Continuar();

			UFUNCTION()
			void OnClicked_BTN_Voltar();

			UFUNCTION()
			void OnSaveGameComplete();

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variaveis

		public:

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_NewGame;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Continuar;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Voltar;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Click;

			UPROPERTY()
			TObjectPtr<class UGame_Save> SaveData;

			TObjectPtr<class IInterface_SaveGame> GameSave_Interface;

			TObjectPtr<class UMain_Game_Instance> GameInstance;

	#pragma endregion
};
