#include "Widgets/Menu/MenuPrincipal/MenuPrincipal_Jogar.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal_Jogar::Init()
{
	SetIsEnabled(true);

	GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());
	if (!GameInstance) return;

	GameInstance->OnSaveGameComplete.BindUObject(this, &UMenuPrincipal_Jogar::OnSaveGameComplete);

	GameData = GameInstance->GetGameData();

	BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);
}


void UMenuPrincipal_Jogar::OnClicked_BTN_NewGame()
{
	if (IsValid(GameInstance))
	{
		GameData = NewObject<UGame_Save>();

		GameInstance->SaveGame(true, GameData);
		GameInstance->SaveGame(false, GameData);

		OnSaveGameComplete();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Menu Principal New Game)"));
	}
}

void UMenuPrincipal_Jogar::OnClicked_BTN_Continuar()
{
	if (GameInstance && GameData)
	{
		UGameplayStatics::OpenLevel(this, GameData->MapaName);
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
	if (!GameData) return;

	UGameplayStatics::OpenLevel(this, GameData->MapaName);
}

void UMenuPrincipal_Jogar::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_NewGame->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);

	if (IsValid(GameInstance)) GameInstance->OnSaveGameComplete.Unbind();
}
