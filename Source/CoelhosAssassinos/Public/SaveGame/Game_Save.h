#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Game_Save.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UGame_Save : public USaveGame
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, Category = "Save")
		FName MapaName = FName("Map_Tutorial");

		UPROPERTY(EditAnywhere, Category = "Save")
		int32 Vida = 3;
};
