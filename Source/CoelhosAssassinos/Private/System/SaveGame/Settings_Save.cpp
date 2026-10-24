#include "System/SaveGame/Settings_Save.h"

USettings_Save::USettings_Save()
{
	Volume_Actors	= 75.f;
	Volume_Effects	= 75.f;
	Volume_Music	= 75.f;
	
	bActorsIsMuted	= false;
	bEffectsIsMuted = false;
	bMusicIsMuted	= false;
	bIsMutedAll		= false;
}
