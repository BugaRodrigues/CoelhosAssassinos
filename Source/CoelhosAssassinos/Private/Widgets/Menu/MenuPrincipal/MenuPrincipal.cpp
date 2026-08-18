#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PC->SetInputMode(InputMode);
	}

	BTN_Jogar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);
	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);

	WS_Menu->SetActiveWidgetIndex(0);
}

void UMenuPrincipal::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_Jogar->OnClicked.RemoveDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);
	BTN_Opcoes->OnClicked.RemoveDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.RemoveDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;

		PC->SetInputMode(InputMode);
	}
}

void UMenuPrincipal::OnClicked_BTN_Jogar()
{
	PlaySound(SB_Click);

	WS_Menu->SetActiveWidgetIndex(1);
}

void UMenuPrincipal::OnClicked_BTN_Opcoes()
{
	PlaySound(SB_Click);

	WS_Menu->SetActiveWidgetIndex(2);
}

void UMenuPrincipal::OnClicked_BTN_Sair()
{
	PlaySound(SB_Click);

	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMenuPrincipal::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}
