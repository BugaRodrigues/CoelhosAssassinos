#include "Widgets/Menu/MenuPrincipal/MenuPrincipal_Jogar.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal_Jogar::Init()
{
	SetIsEnabled(true);

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance) return;

	Interface_SaveGame = Cast<IInterface_SaveGame>(GameInstance);
	if (!Interface_SaveGame) return;

	Interface_SaveGame->OnSaveGameComplete.BindUObject(this, &UMenuPrincipal_Jogar::OnSaveGameComplete);

	GameData = Interface_SaveGame->GetGameData();

	if (GameData->Mapa_Atual.IsNone())
	{
		BTN_Continuar->SetVisibility(ESlateVisibility::Collapsed);
	}

	BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);
}


void UMenuPrincipal_Jogar::OnClicked_BTN_NewGame()
{
	if (Interface_SaveGame)
	{
		GameData = NewObject<UGame_Save>();

		Interface_SaveGame->SaveGame(true, GameData);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Menu Principal New Game)"));
	}
}

void UMenuPrincipal_Jogar::OnClicked_BTN_Continuar()
{
	if (Interface_SaveGame)
	{
		UGameplayStatics::OpenLevel(this, Mapa);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSave_Interface or GameData is null. (Menu Principal Continue)"));
	}
}

void UMenuPrincipal_Jogar::OnClicked_BTN_Voltar()
{
	if (UWidgetSwitcher* WS_Menu = Cast<UWidgetSwitcher>(GetParent()))
	{
		WS_Menu->SetActiveWidgetIndex(0);
		WS_Menu->GetActiveWidget()->SetIsEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WS_Menu is null. (Menu Principal Back)"));
	}
}

void UMenuPrincipal_Jogar::OnSaveGameComplete()
{
	if (Mapa.IsNone()) return;

	UGameplayStatics::OpenLevel(this, Mapa);
}

void UMenuPrincipal_Jogar::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_NewGame->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);

	if (Interface_SaveGame) Interface_SaveGame->OnSaveGameComplete.Unbind();
}
