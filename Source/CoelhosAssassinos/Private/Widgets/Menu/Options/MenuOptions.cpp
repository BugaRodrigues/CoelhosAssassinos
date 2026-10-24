#include "Widgets/Menu/Options/MenuOptions.h"
#include "Widgets/Menu/Options/Controll/Controll_Settings.h"
#include "Widgets/Menu/Options/Controll/TouchOptions.h"
#include "Widgets/Menu/Options/Audio/Audio_Settings.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuOptions::Init()
{
	SetIsEnabled(true);

	WidgetActive = nullptr;

	BTN_Video->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Video);
	BTN_Audio->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Audio);
	BTN_Controls->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Controls);
	BTN_Voltar->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Voltar);

	OnClicked_BTN_Video();
}

void UMenuOptions::OnClicked_BTN_Video()
{
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_MenuOptions->SetActiveWidgetIndex(0);

	WidgetActive = WS_MenuOptions->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);
}

void UMenuOptions::OnClicked_BTN_Audio()
{
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_MenuOptions->SetActiveWidgetIndex(1);

	WidgetActive = WS_MenuOptions->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);

	if (UAudio_Settings* Audio = Cast<UAudio_Settings>(WS_MenuOptions->GetActiveWidget())) Audio->Init();
}

void UMenuOptions::OnClicked_BTN_Controls()
{
#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_MenuOptions->SetActiveWidgetIndex(3);

	WidgetActive = WS_MenuOptions->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);

	if (UTouchOptions* Control = Cast<UTouchOptions>(WS_MenuOptions->GetActiveWidget())) Control->Init();
#elif PLATFORM_DESKTOP
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_MenuOptions->SetActiveWidgetIndex(2);

	WidgetActive = WS_MenuOptions->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);

	if (UControll_Settings* Control = Cast<UControll_Settings>(WS_MenuOptions->GetActiveWidget())) Control->Init();
#endif
}

void UMenuOptions::OnClicked_BTN_Voltar()
{
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	if (UWidgetSwitcher* WS_MenuPrincipal = Cast<UWidgetSwitcher>(GetParent()))
	{
		WS_MenuPrincipal->SetActiveWidgetIndex(0);
		WS_MenuPrincipal->GetActiveWidget()->SetIsEnabled(true);
	}

	WidgetActive = nullptr;
}


void UMenuOptions::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_Video->OnClicked.RemoveAll(this);
	BTN_Audio->OnClicked.RemoveAll(this);
	BTN_Controls->OnClicked.RemoveAll(this);
	BTN_Voltar->OnClicked.RemoveAll(this);
}
