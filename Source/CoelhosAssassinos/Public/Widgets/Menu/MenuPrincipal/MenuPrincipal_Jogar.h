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
			void Init();

		protected:
			UFUNCTION()
			void OnClicked_BTN_NewGame();

			UFUNCTION()
			void OnClicked_BTN_Continuar();

			UFUNCTION()
			void OnClicked_BTN_Voltar();

			UFUNCTION()
			void OnSaveGameComplete();

			virtual void NativeDestruct() override;

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

			UPROPERTY()
			TObjectPtr<class UGame_Save> GameData;

			TObjectPtr<class IInterface_SaveGame> GameSave_Interface;

			TObjectPtr<class UMain_Game_Instance> GameInstance;

	#pragma endregion
};
