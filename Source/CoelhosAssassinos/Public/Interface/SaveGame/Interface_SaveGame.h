#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "System/SaveGame/Game_Save.h"

#include "Interface_SaveGame.generated.h"

DECLARE_DELEGATE(FOnLoadGameComplete)
DECLARE_DELEGATE(FOnSaveGameComplete)

UINTERFACE(MinimalAPI)
class UInterface_SaveGame : public UInterface { GENERATED_BODY() };

class COELHOSASSASSINOS_API IInterface_SaveGame
{
	GENERATED_BODY()
	
	public:
		UFUNCTION()
		virtual void SaveGame(bool bAsync, UGame_Save* NewGameData) = 0;

		UFUNCTION()
		virtual void LoadSave(bool bAsync) = 0;

		UFUNCTION()
		virtual UGame_Save* GetGameData() = 0;

	public:
		FOnLoadGameComplete OnLoadGameComplete;
		FOnSaveGameComplete OnSaveGameComplete;

};
