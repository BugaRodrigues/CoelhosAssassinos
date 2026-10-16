#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Image_DragVisual.generated.h"


UCLASS()
class COELHOSASSASSINOS_API UImage_DragVisual : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions
		protected:
			virtual void NativePreConstruct() override;

			virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

			virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

			virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent) override;

	#pragma endregion

	#pragma region Properties	
		public:
			UPROPERTY(EditAnywhere)
			TObjectPtr<UTexture2D> IconBrush;

			UPROPERTY(EditAnywhere)
			FVector2D IconBrushSize;

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UImage> Img_Icon;

			bool bIsResizing;

			FVector2D InitialTouchPos;
			FVector2D InitialWidgetSize;

	#pragma endregion
};
