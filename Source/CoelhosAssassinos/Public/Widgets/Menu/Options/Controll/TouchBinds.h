#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TouchBinds.generated.h"

DECLARE_DELEGATE(FOnWidgetDestruct);

UCLASS()
class COELHOSASSASSINOS_API UTouchBinds : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions
		public:

		protected:
			virtual void NativeConstruct() override;
			
			UFUNCTION()
			void OnBTN_VoltarClicked();

			void SetNewPositionHUDInterface(int32 Index, FVector2D NewScreenPosition);

			virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

			virtual void NativeDestruct() override;

	#pragma endregion

	#pragma region Variaveis
		public:
			FOnWidgetDestruct OnWidgetDestruct;

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Voltar;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UTouchInterface> TouchInterface;

	#pragma endregion
};
