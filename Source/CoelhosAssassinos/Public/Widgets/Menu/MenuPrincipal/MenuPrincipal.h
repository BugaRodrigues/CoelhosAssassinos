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
			void OnClicked_BTN_Opcoes();

			UFUNCTION()
			void OnClicked_BTN_Sair();

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variables

		public:		
	
		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Jogar;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Opcoes;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Sair;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UWidgetSwitcher> WS_Menu;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Click;

	#pragma endregion

};
