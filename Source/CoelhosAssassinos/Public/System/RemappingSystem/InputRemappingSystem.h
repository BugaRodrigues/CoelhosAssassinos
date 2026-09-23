#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputRemappingSystem.generated.h"

class UEnhancedInputUserSettings;

UCLASS()
class COELHOSASSASSINOS_API UInputRemappingSystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

	public:
		UFUNCTION()
		void RemapKey(FName MappingName, FKey NewKey);

		UFUNCTION()
		FKey GetCurrentKey(FName ActionName) const;

		UFUNCTION()
		void SaveSettings();

	private:
		UEnhancedInputUserSettings* GetUserSettings() const;
};
