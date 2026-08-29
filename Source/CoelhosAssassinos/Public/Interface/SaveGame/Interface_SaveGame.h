#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGame/Game_Save.h"
#include "Interface_SaveGame.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_SaveGame : public UInterface { GENERATED_BODY() };

class COELHOSASSASSINOS_API IInterface_SaveGame
{
	GENERATED_BODY()
	
	public:
		virtual void SaveGame(bool bAsync, UGame_Save* NewGameData) = 0;

		virtual void LoadSave(bool bAsync) = 0;

		virtual UGame_Save* GetGameData() = 0;
};
