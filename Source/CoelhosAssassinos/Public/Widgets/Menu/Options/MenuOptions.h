#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuOptions.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UMenuOptions : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			void Init();

		protected:
			UFUNCTION()
			void OnClicked_BTN_Video();

			UFUNCTION()
			void OnClicked_BTN_Audio();

			UFUNCTION()
			void OnClicked_BTN_Controls();

			UFUNCTION()
			void OnClicked_BTN_Voltar();

			virtual void NativeDestruct() override;

	#pragma endregion

	#pragma region Variaveis

		public:

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Video;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Audio;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Controls;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Voltar;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UWidgetSwitcher> WS_MenuOptions;

			UPROPERTY()
			UWidget* WidgetActive;

	#pragma endregion
};
