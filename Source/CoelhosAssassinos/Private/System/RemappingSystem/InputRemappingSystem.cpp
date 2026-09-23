#include "System/RemappingSystem/InputRemappingSystem.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

UEnhancedInputUserSettings* UInputRemappingSystem::GetUserSettings() const
{
	ULocalPlayer* LP = GetLocalPlayer();
	if (!LP) return nullptr;

	auto* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return nullptr;

	return Subsystem->GetUserSettings();
}

void UInputRemappingSystem::RemapKey(FName MappingName, FKey NewKey)
{
	UEnhancedInputUserSettings* UserSettings = GetUserSettings();
	if (!UserSettings) return;

	FMapPlayerKeyArgs Args;
	Args.MappingName = MappingName;
	Args.NewKey = NewKey;

	FGameplayTagContainer FailureReason;

	UserSettings->MapPlayerKey(Args, FailureReason);
	UserSettings->ApplySettings();
}

FKey UInputRemappingSystem::GetCurrentKey(FName ActionName) const
{
	UEnhancedInputUserSettings* UserSettings = GetUserSettings();
	if (!UserSettings) return EKeys::Invalid;

	UEnhancedPlayerMappableKeyProfile* KeyProfile = UserSettings->GetActiveKeyProfile();
	if (!KeyProfile) return EKeys::Invalid;

	const FKeyMappingRow* KeyMappingRow = KeyProfile->FindKeyMappingRow(ActionName); 
	if (!KeyMappingRow || KeyMappingRow->Mappings.IsEmpty()) return EKeys::Invalid;

	TArray<FPlayerKeyMapping> Mappings = KeyMappingRow->Mappings.Array();

	return Mappings[0].GetCurrentKey();
}

void UInputRemappingSystem::SaveSettings()
{
	UEnhancedInputUserSettings* UserSettings = GetUserSettings();
	if (!UserSettings) return;

	UserSettings->AsyncSaveSettings();
}
