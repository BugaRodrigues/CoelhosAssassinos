#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal::NativeConstruct()
{
	Super::NativeConstruct();

	SaveData = GetGameInstance()->GetSubsystem<IInterface_SaveGame>()->GetSaveData();
	
	BTN_Jogar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);

	BTN_NewGame->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_NewGame);
	BTN_Continuar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Continuar);

	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);
}

void UMenuPrincipal::NativeDestruct()
{
	Super::NativeDestruct();

}

void UMenuPrincipal::OnClicked_BTN_Jogar()
{
	UGameplayStatics::OpenLevel(this, SaveData->MapaName);
}

void UMenuPrincipal::OnClicked_BTN_NewGame()
{
	UGame_Save* NewSaveData = NewObject<UGame_Save>();

	GetGameInstance()->GetSubsystem<IInterface_SaveGame>()->SetSaveData(NewSaveData);

	UGameplayStatics::OpenLevel(this, NewSaveData->MapaName);
}

void UMenuPrincipal::OnClicked_BTN_Continuar()
{
	GetGameInstance()->GetSubsystem<IInterface_SaveGame>()->LoadSave(false);

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
