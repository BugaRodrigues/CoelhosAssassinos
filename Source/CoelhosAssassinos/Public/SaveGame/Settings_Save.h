#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Settings_Save.generated.h"

USTRUCT()
struct FAudioSave
{
	GENERATED_BODY()

	public:
		UPROPERTY()
		float Volume_Music = 75.f;

		UPROPERTY()
		bool bMusicIsMuted = false;

		UPROPERTY()
		float Volume_Actors = 75.f;

		UPROPERTY()
		bool bActorsIsMuted = false;

		UPROPERTY()
		float Volume_Effects = 75.f;

		UPROPERTY()
		bool bEffectsIsMuted = false;

		UPROPERTY()
		bool bIsMutedAll = false;
};

UCLASS()
class COELHOSASSASSINOS_API USettings_Save : public USaveGame
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, category = "Audio")
		FAudioSave Audio;
};
