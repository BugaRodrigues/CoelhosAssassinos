#include "Widgets/Menu/Pause/MenuPause.h"
#include "Character/Controller/Player/Player_Controller.h"
#include "Widgets/Menu/Options/MenuOptions.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPause::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		Pc->SetInputMode(InputMode);
	}

	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Continuar);
	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Sair);

	WS_Menu->SetActiveWidgetIndex(0);
}

void UMenuPause::OnClicked_BTN_Continuar()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	RemoveFromParent();
}

void UMenuPause::OnClicked_BTN_Opcoes()
{
	if (IsValid(LastWidget) && LastWidget->GetIsEnabled()) LastWidget->SetIsEnabled(false);

	LastWidget = WS_Menu->GetActiveWidget();

	WS_Menu->SetActiveWidgetIndex(1);
	WS_Menu->GetActiveWidget()->SetIsEnabled(true);

	if (UMenuOptions* Options = Cast<UMenuOptions>(WS_Menu->GetActiveWidget())) Options->Init();

	LastWidget->SetIsEnabled(false);
}

void UMenuPause::OnClicked_BTN_Sair()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Map_MainMenu"));
}

void UMenuPause::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayer_Controller* PC = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;

		PC->SetInputMode(InputMode);
	}

	BTN_Continuar->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Continuar);
	BTN_Opcoes->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Sair);
}
