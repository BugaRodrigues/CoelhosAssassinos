#include "Widgets/Menu/Options/Controll/TouchBinds.h"
#include "Character/Controller/Player/Player_Controller.h"
#include "Widgets/DragAndDrop/DragAndDropOp.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GameFramework/TouchInterface.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UTouchBinds::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->AddOrRemoveImc(true);
	}

	BTN_Voltar->OnClicked.AddDynamic(this, &UTouchBinds::OnBTN_VoltarClicked);

	SetButtonInterfacePosition(LeftButton);
	SetButtonInterfacePosition(RightButton);
	SetButtonInterfacePosition(UpButton);
}

bool UTouchBinds::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UDragAndDropOp* DragOp = Cast<UDragAndDropOp>(InOperation);
	if (!DragOp || !DragOp->DragedWidget) return false;

	FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	SetButtonInterfacePosition(DragOp->DragedWidget, DropPosition);
	UpdateTouchControlPosition(GetIndexFromWidget(DragOp->DragedWidget), DropPosition);

	DragOp->DragedWidget->SetVisibility(ESlateVisibility::Visible);
	DragOp->DragedWidget->SetRenderOpacity(1.f);

	return true;
}

void UTouchBinds::UpdateTouchControlPosition(int32 Index, FVector2D NewScreenPosition)
{
	if (!TouchInterface || !TouchInterface->Controls.IsValidIndex(Index)) return;

	TouchInterface->Controls[Index].Center = ConertViewportToFloat(NewScreenPosition);
}

void UTouchBinds::SetButtonInterfacePosition(UWidget* Widget, FVector2D NewPosition)
{
	if (!TouchInterface) return;

	int32 Index = GetIndexFromWidget(Widget);

	UCanvasPanelSlot* WidgetSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (!WidgetSlot) return;

	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(Widget);
	if (DPIScale <= 0.f) return;

	WidgetSlot->SetPosition((NewPosition.IsNearlyZero()) ? GetHUDPosition(Index) / DPIScale : NewPosition);
	WidgetSlot->SetSize(TouchInterface->Controls[Index].VisualSize / DPIScale);
}

FVector2D UTouchBinds::ConertViewportToFloat(FVector2D Position)
{
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	const float PostionX = FMath::GetMappedRangeValueClamped(FVector2D(0., ViewportSize.X), FVector2D(0., 1.), Position.X);
	const float PostionY = FMath::GetMappedRangeValueClamped(FVector2D(0., ViewportSize.Y), FVector2D(0., 1.), Position.Y);

	return FVector2D(PostionX, PostionY);
}

FVector2D UTouchBinds::GetHUDPosition(int32 Index)
{
	if (!TouchInterface || !TouchInterface->Controls.IsValidIndex(Index)) return FVector2D(-100., -100.);

	FTouchInputControl& Control = TouchInterface->Controls[Index];

	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	FVector2D Normalized = TouchInterface->Controls[Index].Center;

	return FVector2D(Normalized.X * ViewportSize.X, Normalized.Y * ViewportSize.Y);
}

int32 UTouchBinds::GetIndexFromWidget(UWidget* Widget)
{
	if (Widget == LeftButton)
	{
		return 0;
	}
	else if (Widget == RightButton)
	{
		return 1;
	}
	else if (Widget == UpButton)
	{
		return 2;
	}

	return -1;
}

void UTouchBinds::OnBTN_VoltarClicked()
{
	RemoveFromParent();
}

void UTouchBinds::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->AddOrRemoveImc(false);
	}

	OnWidgetDestruct.ExecuteIfBound();

	BTN_Voltar->OnClicked.RemoveAll(this);
}
