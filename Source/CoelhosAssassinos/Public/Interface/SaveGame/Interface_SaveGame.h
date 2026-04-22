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
		UFUNCTION()
		virtual void OnSaveGame() = 0;

		UFUNCTION()
		virtual void OnLoadGame() = 0;
	
};
