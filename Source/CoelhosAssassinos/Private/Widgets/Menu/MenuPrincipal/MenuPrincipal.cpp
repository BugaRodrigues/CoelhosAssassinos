#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MenuAnchor.h"
#include "Components/Button.h"
#include "Input/Reply.h"

void UMenuPrincipal::NativeConstruct()
{
	Super::NativeConstruct();

	GameSave_Interface = Interface_SG.GetInterface();
	
	if (!GameSave_Interface) { UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null")); return; }

	SaveData = GameSave_Interface->GetSaveData();

	BTN_Jogar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);

	BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Continuar);

	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);

	MenuAnchor_Jogar->Close();
}

FReply UMenuPrincipal::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);

	if (MenuAnchor_Jogar->IsOpen())
	{
		MenuAnchor_Jogar->Close();

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UMenuPrincipal::NativeDestruct()
{
	Super::NativeDestruct();

}

void UMenuPrincipal::OnClicked_BTN_Jogar()
{
	MenuAnchor_Jogar->Open(true);
}

void UMenuPrincipal::OnClicked_BTN_NewGame()
{
	if (!GameSave_Interface) UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null"));

	UGame_Save* NewSaveData = NewObject<UGame_Save>();

	GameSave_Interface->SaveGame(false, NewSaveData);

	UGameplayStatics::OpenLevel(this, NewSaveData->MapaName);
}

void UMenuPrincipal::OnClicked_BTN_Continuar()
{
	if (!GameSave_Interface) UE_LOG(LogTemp, Error, TEXT("GameSave_Interface is null"));

	GameSave_Interface->LoadSave(false);

	UGameplayStatics::OpenLevel(this, SaveData->MapaName);
}

void UMenuPrincipal::OnClicked_BTN_Opcoes()
{
	UE_LOG(LogTemp, Warning, TEXT("Opcoes"));
}

void UMenuPrincipal::OnClicked_BTN_Sair()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
