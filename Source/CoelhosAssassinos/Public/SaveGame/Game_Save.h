#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Game_Save.generated.h"

USTRUCT()
struct FDataSave
{
	GENERATED_BODY()

	UPROPERTY()
	FName PlayerName;
};

UCLASS()
class COELHOSASSASSINOS_API UGame_Save : public USaveGame
{
	GENERATED_BODY()

	public:
		UPROPERTY()
		FDataSave DataSave;
};
