#include "Widgets/Menu/Pause/MenuPause.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPause::NativeConstruct()
{
	Super::NativeConstruct();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
	}

	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Continuar);
	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPause::OnClicked_BTN_Sair);

	WS_Menu->SetActiveWidgetIndex(0);
}

void UMenuPause::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;

		PC->SetInputMode(InputMode);
	}

	BTN_Continuar->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Continuar);
	BTN_Opcoes->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.RemoveDynamic(this, &UMenuPause::OnClicked_BTN_Sair);
}

void UMenuPause::OnClicked_BTN_Continuar()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	RemoveFromParent();
}

void UMenuPause::OnClicked_BTN_Opcoes()
{
	WS_Menu->SetActiveWidgetIndex(1);
}

void UMenuPause::OnClicked_BTN_Sair()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}