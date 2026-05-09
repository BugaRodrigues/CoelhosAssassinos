#include "Widgets/Menu/MenuPrincipal/MenuPrincipal_Jogar.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal_Jogar::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());

	if (IsValid(GameInstance))
	{
		GameInstance->OnSaveGameComplete.BindUObject(this, &UMenuPrincipal_Jogar::OnSaveGameComplete);
	
		GameSave_Interface = Cast<IInterface_SaveGame>(GetGameInstance());
	}

	if (!GameSave_Interface) { UE_LOG(LogTemp, Error, TEXT("GameSave_Interface or GameInstance is null. (Menu Principal Init)")); return; }

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

	if (IsValid(GameInstance)) GameInstance->OnSaveGameComplete.Unbind();
}

void UMenuPrincipal_Jogar::OnClicked_BTN_NewGame()
{
	if (GameSave_Interface)
	{
		SaveData = NewObject<UGame_Save>();

		if (IsValid(GameInstance))
		{ 
			GameSave_Interface->SaveGame(true, SaveData);
		}
		else
		{
			GameSave_Interface->SaveGame(false, SaveData);

			OnSaveGameComplete();
		}
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

void UMenuPrincipal_Jogar::OnSaveGameComplete()
{
	UGameplayStatics::OpenLevel(this, SaveData->MapaName);
}
