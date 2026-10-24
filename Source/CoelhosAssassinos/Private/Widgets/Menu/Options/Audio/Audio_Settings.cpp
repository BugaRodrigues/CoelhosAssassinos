#include "Widgets/Menu/Options/Audio/Audio_Settings.h"
#include "Interface/SaveGame/Interface_SaveSettings.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"

void UAudio_Settings::Init()
{
	SetIsEnabled(true);

	//----------------------------------------Init Interface Save Settings-----------------------------------------
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance) return;

	Interface_SaveSettings = Cast<IInterface_SaveSettings>(GameInstance);
	if (!Interface_SaveSettings) return;

	SettingsData = Interface_SaveSettings->GetSettingsData();

	//----------------------------------------Init Interactives Buttons--------------------------------------------
	Slider_Music->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnMusicValueChange);
	Slider_Actors->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnActorsValueChange);
	Slider_Effects->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnEffectsValueChange);

	CB_MuteAll->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMuteAllChange);
	CB_Music->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMusicMuteChange);
	CB_Actors->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnActorsMuteChange);
	CB_Effects->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnEffectsMuteChange);

	Slider_Music->SetValue(SettingsData->Volume_Music);
	Slider_Actors->SetValue(SettingsData->Volume_Actors);
	Slider_Effects->SetValue(SettingsData->Volume_Effects);

	if (SettingsData->bIsMutedAll)
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
		OnMuteAllChange(SettingsData->bIsMutedAll);
	}
	else
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
		OnMusicMuteChange(SettingsData->bMusicIsMuted);
		OnActorsMuteChange(SettingsData->bActorsIsMuted);
		OnEffectsMuteChange(SettingsData->bEffectsIsMuted);
	}
}

void UAudio_Settings::OnMusicValueChange(float NewValue)
{
	Slider_Music->SetValue(NewValue);

	TB_MusicVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Music, NewValue);

	SettingsData->Volume_Music = NewValue;
}

void UAudio_Settings::OnMusicMuteChange(bool bIsChecked)
{
	if (SettingsData->bIsMutedAll && !bIsChecked)
	{
		SettingsData->bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Music->GetValue() : 0.f);
	SettingsData->bMusicIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Music, TB_MusicVolume, CB_Music, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnActorsValueChange(float NewValue)
{
	Slider_Actors->SetValue(NewValue);

	TB_ActorsVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, NewValue);

	SettingsData->Volume_Actors = NewValue;
}

void UAudio_Settings::OnActorsMuteChange(bool bIsChecked)
{
	if (SettingsData->bIsMutedAll && !bIsChecked)
	{
		SettingsData->bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Actors->GetValue() : 0.f);
	SettingsData->bActorsIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Actors, TB_ActorsVolume, CB_Actors, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnEffectsValueChange(float NewValue)
{
	Slider_Effects->SetValue(NewValue);

	TB_EffectsVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Effects, NewValue);

	SettingsData->Volume_Effects = NewValue;
}

void UAudio_Settings::OnEffectsMuteChange(bool bIsChecked)
{
	if (SettingsData->bIsMutedAll && !bIsChecked)
	{
		SettingsData->bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Effects->GetValue() : 0.f);
	SettingsData->bEffectsIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Effects, TB_EffectsVolume, CB_Effects, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnMuteAllChange(bool bIsChecked)
{
	SettingsData->bIsMutedAll = bIsChecked;

	OnMusicMuteChange(bIsChecked);
	OnActorsMuteChange(bIsChecked);
	OnEffectsMuteChange(bIsChecked);
}

void UAudio_Settings::ActiveDesactveSound(USlider* Slider, UTextBlock* Text, UCheckBox* Check, bool bIsOn)
{
	if (bIsOn)
	{
		Check->SetCheckedState(ECheckBoxState::Checked);
		Slider->SetIsEnabled(false);
		Text->SetIsEnabled(false);
	}
	else
	{
		Check->SetCheckedState(ECheckBoxState::Unchecked);
		Slider->SetIsEnabled(true);
		Text->SetIsEnabled(true);
	}
}

void UAudio_Settings::NativeDestruct()
{
	Super::NativeDestruct();

	if (Interface_SaveSettings) Interface_SaveSettings->SaveSettings(false, SettingsData);

	Slider_Music->OnValueChanged.RemoveAll(this);
	Slider_Actors->OnValueChanged.RemoveAll(this);
	Slider_Effects->OnValueChanged.RemoveAll(this);

	CB_Music->OnCheckStateChanged.RemoveAll(this);
	CB_Actors->OnCheckStateChanged.RemoveAll(this);
	CB_Effects->OnCheckStateChanged.RemoveAll(this);
}
