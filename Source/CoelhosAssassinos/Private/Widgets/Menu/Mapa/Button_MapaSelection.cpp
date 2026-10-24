#include "Widgets/Menu/Mapa/Button_MapaSelection.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UButton_MapaSelection::NativeConstruct()
{
	Super::NativeConstruct();

	if (!TextIndice.IsEmpty()) Tb_MapIndice->SetText(TextIndice);

	switch (StateLevel)
	{
		case ELevelState::Completo:
			Btn_Mapa->SetBackgroundColor(FLinearColor::Green);
			Btn_Mapa->SetRenderOpacity(.75f);
			break;
		case ELevelState::Atual:
			Btn_Mapa->SetBackgroundColor(FLinearColor::Blue);
			break;
		case ELevelState::Bloqueado:
			Btn_Mapa->SetBackgroundColor(FLinearColor::Black);
			Btn_Mapa->SetRenderOpacity(.5f);
			Btn_Mapa->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
	}

	Btn_Mapa->OnClicked.AddDynamic(this, &UButton_MapaSelection::OnClickMapa);
}

void UButton_MapaSelection::OnClickMapa()
{
	if (Mapa.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Mapa is not Defined!"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, Mapa);
	}
}

void UButton_MapaSelection::NativeDestruct()
{
	Super::NativeDestruct();

	Btn_Mapa->OnClicked.RemoveAll(this);
}