#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "GameFramework/PlayerController.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Input/Reply.h"

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

	GameSave_Interface = Cast<IInterface_SaveGame>(GetGameInstance());
	
	if (!GameSave_Interface) { UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Menu Principal Init)")); return; }

	SaveData = GameSave_Interface->GetSaveData();

	#pragma region Menu Principal Buttons Bind
		BTN_Jogar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);
		BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
		BTN_Sair->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);
	#pragma endregion

	#pragma region Menu Jogar Buttons Bind
		BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_NewGame);
		BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Continuar);
		BTN_Voltar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Voltar);
	#pragma endregion

	#pragma region Menu Options Buttons Bind
		BTN_Video->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Video);
		BTN_Audio->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Audio);
		BTN_Controls->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Controls);
		BTN_Voltar_2->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Voltar);
	#pragma endregion

	WS_Menu->SetActiveWidgetIndex(0);
}

void UMenuPrincipal::NativeDestruct()
{
	Super::NativeDestruct();

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;

		PC->SetInputMode(InputMode);
	}
}

#pragma region Menu Principal Buttons

	void UMenuPrincipal::OnClicked_BTN_Jogar()
	{
		WS_Menu->SetActiveWidgetIndex(1);
	}

	void UMenuPrincipal::OnClicked_BTN_Opcoes()
	{
		WS_Menu->SetActiveWidgetIndex(2);
	}

	void UMenuPrincipal::OnClicked_BTN_Sair()
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
	}

#pragma endregion

#pragma region Menu Jogar Buttons

	void UMenuPrincipal::OnClicked_BTN_NewGame()
	{
		if (!GameSave_Interface) UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Button New Game)"));

		UGame_Save* NewSaveData = NewObject<UGame_Save>();

		GameSave_Interface->SaveGame(false, NewSaveData);

		UGameplayStatics::OpenLevel(this, NewSaveData->MapaName);
	}

	void UMenuPrincipal::OnClicked_BTN_Continuar()
	{
		if (!GameSave_Interface) UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null. (Button Continue)"));

		GameSave_Interface->LoadSave(false);

		UGameplayStatics::OpenLevel(this, SaveData->MapaName);
	}

#pragma endregion

#pragma region Menu Options Buttons

	void UMenuPrincipal::OnClicked_BTN_Video()
	{
	}

	void UMenuPrincipal::OnClicked_BTN_Audio()
	{
	}

	void UMenuPrincipal::OnClicked_BTN_Controls()
	{
	}

#pragma endregion

void UMenuPrincipal::OnClicked_BTN_Voltar()
{
	WS_Menu->SetActiveWidgetIndex(0);
}
