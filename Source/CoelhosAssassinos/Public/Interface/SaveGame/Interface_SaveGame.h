#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_SaveGame.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_SaveGame : public UInterface { GENERATED_BODY() };

class COELHOSASSASSINOS_API IInterface_SaveGame
{
	GENERATED_BODY()
	
	public:
		virtual void SaveGame(bool bAsync) = 0;

		virtual void LoadGame(bool bAsync) = 0;

		virtual class UGame_Save* GetGameData() = 0;
	
};
