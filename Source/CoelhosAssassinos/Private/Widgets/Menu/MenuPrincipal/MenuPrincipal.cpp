#include "Widgets/Menu/MenuPrincipal/MenuPrincipal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuPrincipal::NativeConstruct()
{
	Super::NativeConstruct();
	
	BTN_Jogar->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Jogar);
	BTN_Opcoes->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Opcoes);
	BTN_Sair->OnClicked.AddDynamic(this, &UMenuPrincipal::OnClicked_BTN_Sair);
}

void UMenuPrincipal::NativeDestruct()
{
	Super::NativeDestruct();

}

void UMenuPrincipal::OnClicked_BTN_Jogar()
{
	UGameplayStatics::OpenLevel(this, FName(*Mapa.GetAssetName()));
}

void UMenuPrincipal::OnClicked_BTN_Opcoes()
{
	UE_LOG(LogTemp, Warning, TEXT("Opcoes"));
}

void UMenuPrincipal::OnClicked_BTN_Sair()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
