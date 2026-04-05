#include "GameMode/MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Objetos/Chave/Chave.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	ChavesTotais = 0;
	ChavesColetadas = 0;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ChavesTotais = GetAllKeys();

	if (ChavesTotais > 0)
	{
		AddWidget();

		ShowKeys.ExecuteIfBound(ChavesTotais, ChavesColetadas);
	}
	else
	{
		OpenGate.ExecuteIfBound();
	}
}

int32 AMainGameMode::GetAllKeys()
{
	TArray<AActor*> ChavesEncontradas;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AChave::StaticClass(), ChavesEncontradas);

	if (ChavesEncontradas.Num() > 0)
	{
		for (AActor* Actor : ChavesEncontradas)
		{
			Cast<AChave>(Actor)->OnKeyCollected.BindUObject(this, &AMainGameMode::OnKeyCollected);
		}
	}

	return ChavesEncontradas.Num();
}

void AMainGameMode::AddWidget()
{
	if (HUD_WidgetClass)
	{
		HUD_Widget = CreateWidget<UUserWidget>(GetWorld(), HUD_WidgetClass);

		if (HUD_Widget)
		{
			HUD_Widget->AddToViewport();
		}
	}
}

void AMainGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (Chave)
	{
		Chave->OnKeyCollected.Unbind();
	}
}
