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

			void RefreshButtonsPosition();

			void UpdateTouchControlPosition(int32 Index, FVector2D NewScreenPosition);

			virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

			void SetButtonInterfacePosition(UWidget* Widget, FVector2D NewPosition = FVector2D::ZeroVector);

			virtual void NativeDestruct() override;

			FVector2D ConertViewportToFloat(FVector2D Position);

			FVector2D GetHUDPosition(int32 Index);

			int32 GetIndexFromWidget(UWidget* Widget);

	#pragma endregion

	#pragma region Variaveis
		public:
			FOnWidgetDestruct OnWidgetDestruct;

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UWidget> LeftButton;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UWidget> RightButton;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UWidget> UpButton;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UWidget> MenuButton;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UButton> BTN_Voltar;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UTouchInterface> TouchInterface;

	#pragma endregion
};
