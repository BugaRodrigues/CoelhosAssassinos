#include "Widgets/Menu/Options/Controll/TouchBinds.h"
#include "Widgets/DragAndDrop/DragAndDropOp.h"
#include "GameFramework/TouchInterface.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"

void UTouchBinds::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Voltar->OnClicked.AddDynamic(this, &UTouchBinds::OnBTN_VoltarClicked);
}

bool UTouchBinds::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragAndDropOp* DragOp = Cast<UDragAndDropOp>(InOperation);
	if (!DragOp || !DragOp->DragedWidget) return false;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(DragOp->DragedWidget->Slot);
	if (!CanvasSlot) return false;

	FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	CanvasSlot->SetPosition(DropPosition);

	DragOp->DragedWidget->SetVisibility(ESlateVisibility::Visible);
	DragOp->DragedWidget->SetRenderOpacity(1.f);

	return true;
}

void UTouchBinds::SetNewPositionHUDInterface(int32 Index, FVector2D NewScreenPosition)
{
	if (!TouchInterface) return;

	TouchInterface->Controls[Index].Center = NewScreenPosition;

	TouchInterface->SaveConfig();
}

void UTouchBinds::OnBTN_VoltarClicked()
{
	RemoveFromParent();
}

void UTouchBinds::NativeDestruct()
{
	Super::NativeDestruct();

	OnWidgetDestruct.ExecuteIfBound();

	BTN_Voltar->OnClicked.RemoveAll(this);
}
