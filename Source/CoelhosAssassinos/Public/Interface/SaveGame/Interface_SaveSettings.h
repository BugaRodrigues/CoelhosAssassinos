#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveGame/Settings_Save.h"
#include "Interface_SaveSettings.generated.h"

UINTERFACE(MinimalAPI)
class UInterface_SaveSettings : public UInterface { GENERATED_BODY() };

class COELHOSASSASSINOS_API IInterface_SaveSettings
{
	GENERATED_BODY()

	public:
		virtual void SaveSettings(bool bAsync, USettings_Save* NewSaveSettings) = 0;

		virtual void LoadSettings(bool bAsync) = 0;

		virtual USettings_Save* GetSettingsData() = 0;
	
};
