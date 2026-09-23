#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "Widgets/Menu/MenuPrincipal/MenuPrincipal_Jogar.h"
#include "Widgets/Menu/Options/MenuOptions.h"
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

void UMenuPrincipal::OnClicked_BTN_Jogar()
{
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_Menu->SetActiveWidgetIndex(1);

	WidgetActive = WS_Menu->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);

	if (UMenuPrincipal_Jogar* Jogar = Cast<UMenuPrincipal_Jogar>(WS_Menu->GetActiveWidget())) Jogar->Init();
}

void UMenuPrincipal::OnClicked_BTN_Opcoes()
{
	if (IsValid(WidgetActive) && WidgetActive->GetIsEnabled()) WidgetActive->SetIsEnabled(false);

	WS_Menu->SetActiveWidgetIndex(2);

	WidgetActive = WS_Menu->GetActiveWidget();
	WidgetActive->SetIsEnabled(true);

	if (UMenuOptions* Options = Cast<UMenuOptions>(WS_Menu->GetActiveWidget())) Options->Init();
}

void UMenuPrincipal::OnClicked_BTN_Sair()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
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
