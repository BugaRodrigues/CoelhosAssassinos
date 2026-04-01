#include "Widgets/Contador/ContadorDeChaves.h"
#include "GameMode/MainGameMode.h"
#include "Components/TextBlock.h"

void UContadorDeChaves::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ShowKeys.BindUObject(this, &UContadorDeChaves::UpdateContador);
	}
}

void UContadorDeChaves::UpdateContador(int32 TotalChaves, int32 ChavesColetadas)
{
	if (!Text_Contador) return;

	Text_Contador->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), ChavesColetadas, TotalChaves)));

	if (ChavesColetadas >= TotalChaves)
	{
		Text_Contador->SetText(FText::FromString(TEXT("Porta aberta!")));
	}
}

void UContadorDeChaves::NativeDestruct()
{
	Super::NativeDestruct();

	if (GameMode)
	{
		GameMode->ShowKeys.Unbind();
	}
}
