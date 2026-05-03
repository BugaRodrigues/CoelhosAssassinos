#include "Widgets/Menu/MenuPrincipal/MenuPrincipal_Jogar.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal_Jogar::NativeConstruct()
{
	Super::NativeConstruct();

	GameSave_Interface = Cast<IInterface_SaveGame>(GetGameInstance());

	if (!GameSave_Interface) { UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Menu Principal Init)")); return; }

	SaveData = GameSave_Interface->GetSaveData();

	BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.AddDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);
}

void UMenuPrincipal_Jogar::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_NewGame->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Continuar);
	BTN_Voltar->OnClicked.RemoveDynamic(this, &UMenuPrincipal_Jogar::OnClicked_BTN_Voltar);
}

void UMenuPrincipal_Jogar::OnClicked_BTN_NewGame()
{
	if (GameSave_Interface)
	{
		UGame_Save* NewSaveData = Cast<UGame_Save>(NewObject<UGame_Save>());

		GameSave_Interface->SaveGame(false, NewSaveData);

		UGameplayStatics::OpenLevel(this, NewSaveData->MapaName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Menu Principal New Game)"));
	}
}

void UMenuPrincipal_Jogar::OnClicked_BTN_Continuar()
{
	if (GameSave_Interface && SaveData)
	{
		UGameplayStatics::OpenLevel(this, SaveData->MapaName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameSave_Interface or SaveData is null. (Menu Principal Continue)"));
	}
}

void UMenuPrincipal_Jogar::OnClicked_BTN_Voltar()
{
	if (UWidgetSwitcher* WS_Menu = Cast<UWidgetSwitcher>(GetParent()))
	{
		WS_Menu->SetActiveWidgetIndex(0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WS_Menu is null. (Menu Principal Back)"));
	}
}
