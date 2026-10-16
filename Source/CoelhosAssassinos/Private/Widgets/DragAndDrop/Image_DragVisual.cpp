#include "Widgets/DragAndDrop/Image_DragVisual.h"
#include "Widgets/DragAndDrop/DragAndDropOp.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"

void UImage_DragVisual::NativePreConstruct()
{
	Super::NativeConstruct();

	IconBrushSize = IconBrushSize.IsZero() ? FVector2D(100., 100.) : IconBrushSize;

	if (!Img_Icon || !IconBrush) return;
	
	Img_Icon->SetBrushFromTexture(IconBrush);
	Img_Icon->SetBrushSize(IconBrushSize);
}

FReply UImage_DragVisual::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
}

void UImage_DragVisual::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragAndDropOp* DragOp = NewObject<UDragAndDropOp>(this);
	if (!DragOp) return;

	DragOp->DragedWidget = this;
	DragOp->DefaultDragVisual = this;
	DragOp->Pivot = EDragPivot::CenterCenter;

	OutOperation = DragOp;

	SetVisibility(ESlateVisibility::HitTestInvisible);
	SetRenderOpacity(0.5f);
}

FReply UImage_DragVisual::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InTouchEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InTouchEvent);

	return UWidgetBlueprintLibrary::DetectDragIfPressed(InTouchEvent, this, EKeys::TouchKeys[0]).NativeReply;
}
