#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TouchOptions.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UTouchOptions : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions
		public:
			void Init();

		protected:
			UFUNCTION()
			void OnBTN_RemapBindsClicked();

			virtual void NativeDestruct() override;

			UFUNCTION()
			void OnTouchBindsDestruct();

	#pragma endregion

	#pragma region Variaveis
		public:

		protected:
			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UTouchBinds> TouchBindsClass;

			UPROPERTY()
			TObjectPtr<class UTouchBinds> TouchBinds;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_RemapBinds;

	#pragma endregion
};
