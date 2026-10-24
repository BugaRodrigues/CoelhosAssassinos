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
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	Super::NativeConstruct();

	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(GetOwningPlayer()))
	{
		Pc->AddOrRemoveImc(true);
	}

	BTN_Voltar->OnClicked.AddDynamic(this, &UTouchBinds::OnBTN_VoltarClicked);

	RefreshButtonsPosition();
#endif
}

void UTouchBinds::RefreshButtonsPosition()
{
	TArray<UWidget*> Buttons = {LeftButton, RightButton, UpButton, MenuButton};

	for (UWidget* Btn : Buttons)
	{
		SetButtonInterfacePosition(Btn);
	}
}

bool UTouchBinds::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	UDragAndDropOp* DragOp = Cast<UDragAndDropOp>(InOperation);
	if (!DragOp || !DragOp->DragedWidget) return false;

	FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	SetButtonInterfacePosition(DragOp->DragedWidget, DropPosition);
	UpdateTouchControlPosition(GetIndexFromWidget(DragOp->DragedWidget), DropPosition);

	DragOp->DragedWidget->SetVisibility(ESlateVisibility::Visible);
	DragOp->DragedWidget->SetRenderOpacity(1.f);

	return true;
#else
	return false;
#endif
}

void UTouchBinds::UpdateTouchControlPosition(int32 Index, FVector2D NewScreenPosition)
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	if (!TouchInterface || !TouchInterface->Controls.IsValidIndex(Index)) return;

	TouchInterface->Controls[Index].Center = ConertViewportToFloat(NewScreenPosition);
#endif
}

void UTouchBinds::SetButtonInterfacePosition(UWidget* Widget, FVector2D NewPosition)
{
#if  PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	if (!TouchInterface) return;

	int32 Index = GetIndexFromWidget(Widget);
	if (!TouchInterface->Controls.IsValidIndex(Index)) return;

	UCanvasPanelSlot* WidgetSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (!WidgetSlot) return;

	float DPIScale = UWidgetLayoutLibrary::GetViewportScale(Widget);
	if (DPIScale <= 0.f) return;

	if (NewPosition.IsNearlyZero())
	{
		FVector2D NormalizedCenter = TouchInterface->Controls[Index].Center;
		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

		WidgetSlot->SetPosition((NormalizedCenter * ViewportSize) / DPIScale);
	}
	else
	{
		WidgetSlot->SetPosition(NewPosition);
	}

	WidgetSlot->SetSize(TouchInterface->Controls[Index].VisualSize / DPIScale);
#endif
}

void UTouchBinds::OnBTN_VoltarClicked()
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	RemoveFromParent();
#endif
}

void UTouchBinds::NativeDestruct()
{
	Super::NativeDestruct();
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->AddOrRemoveImc(false);
	}

	OnWidgetDestruct.ExecuteIfBound();

	BTN_Voltar->OnClicked.RemoveAll(this);
#endif
}

FVector2D UTouchBinds::ConertViewportToFloat(FVector2D Position)
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

	FVector2D AbsolutePos = Position * Scale;

	float PositionX = FMath::Clamp(AbsolutePos.X / ViewportSize.X, 0.f, 1.f);
	float PositionY = FMath::Clamp(AbsolutePos.Y / ViewportSize.Y, 0.f, 1.f);

	/*
	const float PostionX = FMath::GetMappedRangeValueClamped(FVector2D(0., ViewportSize.X), FVector2D(0., 1.), Position.X);
	const float PostionY = FMath::GetMappedRangeValueClamped(FVector2D(0., ViewportSize.Y), FVector2D(0., 1.), Position.Y);
	*/

	return FVector2D(PositionX, PositionY);
#else
	return FVector2D::ZeroVector;
#endif
}

FVector2D UTouchBinds::GetHUDPosition(int32 Index)
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	if (!TouchInterface || !TouchInterface->Controls.IsValidIndex(Index)) return FVector2D(-100., -100.);

	FTouchInputControl& Control = TouchInterface->Controls[Index];

	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);
	FVector2D Normalized = TouchInterface->Controls[Index].Center;

	return FVector2D(Normalized.X * ViewportSize.X, Normalized.Y * ViewportSize.Y);
#else
	return FVector2D::ZeroVector;
#endif
}

int32 UTouchBinds::GetIndexFromWidget(UWidget* Widget)
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
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
	else if (Widget == MenuButton)
	{
		return 3;
	}
#endif
	return -1;
}
