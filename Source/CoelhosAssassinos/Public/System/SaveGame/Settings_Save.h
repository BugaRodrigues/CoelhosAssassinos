#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Settings_Save.generated.h"

UCLASS()
class COELHOSASSASSINOS_API USettings_Save : public USaveGame
{
	GENERATED_BODY()

	public:	
		USettings_Save();

	public:
		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Value")
		float Volume_Music;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Value")
		float Volume_Actors;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Value")
		float Volume_Effects;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Muted")
		bool bMusicIsMuted;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Muted")
		bool bActorsIsMuted;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Muted")
		bool bEffectsIsMuted;

		UPROPERTY(EditAnywhere, SaveGame, Category = "Audio | Muted")
		bool bIsMutedAll;
};
