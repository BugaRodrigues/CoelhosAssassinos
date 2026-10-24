#include "Widgets/Menu/Mapa/MapaSelection.h"
#include "Widgets/Menu/Mapa/Button_MapaSelection.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMapaSelection::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Voltar->OnClicked.AddDynamic(this, &UMapaSelection::OnClicked_BtnVoltar);

	if (!Class_Btn_Mapa) return;

	SetLevelsButtons();
}

void UMapaSelection::SetLevelsButtons()
{
	UGame_Save* GameData = GetGameData();
	if (!GameData) return;

	FName MapaReferencia = (GameData->Mapa_Atual.IsNone()) ? GameData->Mapa_Inicial : GameData->Mapa_Atual;

	UButton_MapaSelection* Btn_Mapa;
	UCanvasPanelSlot* Canvas;

	double PosX = 100.;
	int32 Index = 1, Inversor = 1;

	for (FName Mapa : Mapas)
	{
		Btn_Mapa = CreateWidget<UButton_MapaSelection>(this, Class_Btn_Mapa);
		if (!Btn_Mapa) return;

		Btn_Mapa->Mapa = Mapa;
		Btn_Mapa->TextIndice = FText::FromString(FString::FromInt(Index++));

		if (GameData->Mapas_Completos.Contains(Mapa))
		{
			Btn_Mapa->StateLevel = ELevelState::Completo;
		}
		else if (Mapa.IsEqual(MapaReferencia))
		{
			Btn_Mapa->StateLevel = ELevelState::Atual;
		}
		else
		{
			Btn_Mapa->StateLevel = ELevelState::Bloqueado;
		}

		Canvas = MainCanvas->AddChildToCanvas(Btn_Mapa);
		Canvas->SetAnchors(FAnchors(0.f, .5f));
		Canvas->SetAlignment(FVector2D(1., .5));
		Canvas->SetPosition(FVector2D(PosX += 225., 100 * (Inversor *= -1)));
	}
}

void UMapaSelection::OnClicked_BtnVoltar()
{
	if (Map_MainMenu.IsNone())
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Menu Principal nao definido!"));
	}
	else
	{
		UGameplayStatics::OpenLevel(this, Map_MainMenu);
	}
}

void UMapaSelection::NativeDestruct()
{
	Super::NativeDestruct();

	Btn_Voltar->OnClicked.RemoveAll(this);
}

UGame_Save* UMapaSelection::GetGameData()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance) return nullptr;

	IInterface_SaveGame* Interface = Cast<IInterface_SaveGame>(GameInstance);
	if (!Interface) return nullptr;

	return Interface->GetGameData();
}
