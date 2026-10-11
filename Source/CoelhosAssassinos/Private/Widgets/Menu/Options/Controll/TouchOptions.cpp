#include "Widgets/Menu/Options/Controll/TouchOptions.h"
#include "Widgets/Menu/Options/Controll/TouchBinds.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"

void UTouchOptions::Init()
{
	BTN_RemapBinds->OnClicked.AddDynamic(this, &UTouchOptions::OnBTN_RemapBindsClicked);
}

void UTouchOptions::OnBTN_RemapBindsClicked()
{
	if (!TouchBindsClass) return;

	TouchBinds = CreateWidget<UTouchBinds>(this, TouchBindsClass);

	TouchBinds->AddToViewport();
	TouchBinds->OnWidgetDestruct.BindUObject(this, &UTouchOptions::OnTouchBindsDestruct);

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUserWidget::StaticClass(), true);

	if (FoundWidgets.IsValidIndex(0)) FoundWidgets[0]->SetVisibility(ESlateVisibility::Collapsed);
}

void UTouchOptions::OnTouchBindsDestruct()
{
	if (!TouchBinds) return;

	TouchBinds->OnWidgetDestruct.Unbind();
	TouchBinds = nullptr;

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UUserWidget::StaticClass(), true);

	if (FoundWidgets.IsValidIndex(0)) FoundWidgets[0]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UTouchOptions::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_RemapBinds->OnClicked.RemoveAll(this);
}
