#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Game_Save.generated.h"

UCLASS(Blueprintable)
class COELHOSASSASSINOS_API UGame_Save : public USaveGame
{
	GENERATED_BODY()

	public:
		UGame_Save();

	public:
		UPROPERTY(EditAnywhere, SaveGame, Category = "Mapa")
		FName Mapa_Inicial;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Mapa")
		FName Mapa_Atual;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Mapa")
		TArray<FName> Mapas_Completos;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Save")
		int32 Vida;
};
