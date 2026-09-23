#include "Widgets/Menu/Options/Audio/Audio_Settings.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Kismet/GameplayStatics.h" 
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"

void UAudio_Settings::Init()
{
	SetIsEnabled(true);

	#pragma region Save Game
		GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());
		if (GameInstance)
		{
			Interface_SaveSettings = Cast<IInterface_SaveSettings>(GameInstance);
			if (Interface_SaveSettings)
			{
				SettingsData = Interface_SaveSettings->GetSettingsData();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Interface nao encontrada!!!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game Instance nao encontrada!!!"));
		}
	#pragma endregion

	Slider_Music->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnMusicValueChange);
	Slider_Actors->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnActorsValueChange);
	Slider_Effects->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnEffectsValueChange);

	CB_MuteAll->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMuteAllChange);
	CB_Music->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMusicMuteChange);
	CB_Actors->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnActorsMuteChange);
	CB_Effects->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnEffectsMuteChange);

	FAudioSave Audio = SettingsData->Audio;

	Slider_Music->SetValue(Audio.Volume_Music);
	Slider_Actors->SetValue(Audio.Volume_Actors);
	Slider_Effects->SetValue(Audio.Volume_Effects);

	if (Audio.bIsMutedAll)
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
		OnMuteAllChange(Audio.bIsMutedAll);
	}
	else
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
		OnMusicMuteChange(Audio.bMusicIsMuted);
		OnActorsMuteChange(Audio.bActorsIsMuted);
		OnEffectsMuteChange(Audio.bEffectsIsMuted);
	}
}

void UAudio_Settings::OnMusicValueChange(float NewValue)
{
	Slider_Music->SetValue(NewValue);

	TB_MusicVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Music, NewValue);

	SettingsData->Audio.Volume_Music = NewValue;
}

void UAudio_Settings::OnMusicMuteChange(bool bIsChecked)
{
	if (SettingsData->Audio.bIsMutedAll && !bIsChecked)
	{
		SettingsData->Audio.bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Music->GetValue() : 0.f);
	SettingsData->Audio.bMusicIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Music, TB_MusicVolume, CB_Music, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->Audio.bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnActorsValueChange(float NewValue)
{
	Slider_Actors->SetValue(NewValue);

	TB_ActorsVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, NewValue);

	SettingsData->Audio.Volume_Actors = NewValue;
}

void UAudio_Settings::OnActorsMuteChange(bool bIsChecked)
{
	if (SettingsData->Audio.bIsMutedAll && !bIsChecked)
	{
		SettingsData->Audio.bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Actors->GetValue() : 0.f);
	SettingsData->Audio.bActorsIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Actors, TB_ActorsVolume, CB_Actors, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->Audio.bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnEffectsValueChange(float NewValue)
{
	Slider_Effects->SetValue(NewValue);

	TB_EffectsVolume->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), NewValue)));

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Effects, NewValue);

	SettingsData->Audio.Volume_Effects = NewValue;
}

void UAudio_Settings::OnEffectsMuteChange(bool bIsChecked)
{
	if (SettingsData->Audio.bIsMutedAll && !bIsChecked)
	{
		SettingsData->Audio.bIsMutedAll = false;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
	}

	UGameplayStatics::SetSoundMixClassOverride(this, SM_Target, SC_Actors, (bIsChecked) ? Slider_Effects->GetValue() : 0.f);
	SettingsData->Audio.bEffectsIsMuted = bIsChecked;

	ActiveDesactveSound(Slider_Effects, TB_EffectsVolume, CB_Effects, bIsChecked);

	if (CB_Music->IsChecked() && CB_Actors->IsChecked() && CB_Effects->IsChecked())
	{
		SettingsData->Audio.bIsMutedAll = true;
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
	}
}

void UAudio_Settings::OnMuteAllChange(bool bIsChecked)
{
	SettingsData->Audio.bIsMutedAll = bIsChecked;

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

	if (GameInstance && Interface_SaveSettings) Interface_SaveSettings->SaveSettings(false, SettingsData);

	Slider_Music->OnValueChanged.RemoveAll(this);
	Slider_Actors->OnValueChanged.RemoveAll(this);
	Slider_Effects->OnValueChanged.RemoveAll(this);

	CB_Music->OnCheckStateChanged.RemoveAll(this);
	CB_Actors->OnCheckStateChanged.RemoveAll(this);
	CB_Effects->OnCheckStateChanged.RemoveAll(this);
}
