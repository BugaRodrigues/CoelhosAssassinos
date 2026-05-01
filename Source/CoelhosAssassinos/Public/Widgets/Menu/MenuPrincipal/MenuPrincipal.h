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

			UFUNCTION()
			void OnClicked_BTN_Jogar();

			UFUNCTION()
			void OnClicked_BTN_NewGame();

			UFUNCTION()
			void OnClicked_BTN_Continuar();

			UFUNCTION()
			void OnClicked_BTN_Opcoes();

			UFUNCTION()
			void OnClicked_BTN_Sair();

			virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	#pragma endregion

	#pragma region Variables

		public:		
	
		protected:

			#pragma region Buttons

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Jogar;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_NewGame;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Continuar;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Opcoes;

				UPROPERTY(meta = (BindWidget))
				class UButton* BTN_Sair;

			#pragma endregion

			UPROPERTY(meta = (BindWidget))
			class UMenuAnchor* MenuAnchor_Jogar;

			UPROPERTY(EditAnywhere, Category = "Menu")
			TSoftObjectPtr<UWorld> Mapa;

			UPROPERTY()
			class UGame_Save* SaveData;

			UPROPERTY()
			TScriptInterface<class IInterface_SaveGame> Interface_SG;

			class IInterface_SaveGame* GameSave_Interface;

	#pragma endregion

};
