#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPrincipal.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UMenuPrincipal : public UUserWidget
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	

		protected:
			virtual void NativeConstruct() override;

			virtual void NativeDestruct() override;

			#pragma region Menu Principal Buttons

				UFUNCTION()
				void OnClicked_BTN_Jogar();

				UFUNCTION()
				void OnClicked_BTN_Opcoes();

				UFUNCTION()
				void OnClicked_BTN_Sair();

			#pragma endregion

			#pragma region Menu Jogar Buttons

				UFUNCTION()
				void OnClicked_BTN_NewGame();

				UFUNCTION()
				void OnClicked_BTN_Continuar();

			#pragma endregion

			#pragma region Menu Options Buttons
				
				UFUNCTION()
				void OnClicked_BTN_Video();

				UFUNCTION()
				void OnClicked_BTN_Audio();

				UFUNCTION()
				void OnClicked_BTN_Controls();

			#pragma endregion

			UFUNCTION()
			void OnClicked_BTN_Voltar();

	#pragma endregion

	#pragma region Variables

		public:		
	
		protected:

			#pragma region Menu Principal Buttons

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Jogar;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Opcoes;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Sair;

			#pragma endregion

			#pragma region Menu Jogar Buttons

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_NewGame;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Continuar;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Voltar;

			#pragma endregion

			#pragma region Menu Options Buttons

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Video;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Audio;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Controls;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Voltar_2;

			#pragma endregion

			UPROPERTY(meta = (BindWidget))
			class UWidgetSwitcher* WS_Menu;

			UPROPERTY()
			class UGame_Save* SaveData;

			class IInterface_SaveGame* GameSave_Interface;

	#pragma endregion

};
