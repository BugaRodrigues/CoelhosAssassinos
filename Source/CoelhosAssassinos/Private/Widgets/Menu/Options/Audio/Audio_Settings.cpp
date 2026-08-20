#include "Widgets/Menu/Options/Audio/Audio_Settings.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"

void UAudio_Settings::NativeConstruct()
{
	Super::NativeConstruct();

	#pragma region Sliders
		Slider_Music->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnMusicValueChange);
		Slider_Music->SetValue(VolumeMusic);

		Slider_Actors->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnActorsValueChange);
		Slider_Actors->SetValue(VolumeActors);

		Slider_Effects->OnValueChanged.AddDynamic(this, &UAudio_Settings::OnEffectsValueChange);
		Slider_Effects->SetValue(VolumeEffects);
	#pragma endregion

	#pragma region Check Box
		CB_Music->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMusicMuteChange);
		CB_Music->SetCheckedState(bIsMuteMusic ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

		CB_Actors->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnActorsMuteChange);
		CB_Actors->SetCheckedState(bIsMuteActors ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

		CB_Effects->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnEffectsMuteChange);
		CB_Effects->SetCheckedState(bIsMuteEffects ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

		CB_MuteAll->OnCheckStateChanged.AddDynamic(this, &UAudio_Settings::OnMuteAllChange);
		CB_MuteAll->SetCheckedState(bIsMuteAll ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	#pragma endregion
}

void UAudio_Settings::OnMusicValueChange(float NewValue)
{
	Slider_Music->SetValue(NewValue);

	VolumeMusic = NewValue;
}

void UAudio_Settings::OnMusicMuteChange(bool bIsChecked)
{
	if (bIsChecked)
	{
		CB_Music->SetCheckedState(ECheckBoxState::Checked);

		Slider_Music->SetIsEnabled(false);

		VolumeMusic = 0.f;
	}
	else
	{
		CB_Music->SetCheckedState(ECheckBoxState::Unchecked);

		Slider_Music->SetIsEnabled(true);

		VolumeMusic = Slider_Music->GetValue();
	}
	bIsMuteMusic = bIsChecked;
}

void UAudio_Settings::OnActorsValueChange(float NewValue)
{
	Slider_Actors->SetValue(NewValue);

	VolumeActors = NewValue;
}

void UAudio_Settings::OnActorsMuteChange(bool bIsChecked)
{
	if (bIsChecked)
	{
		CB_Actors->SetCheckedState(ECheckBoxState::Checked);

		Slider_Actors->SetIsEnabled(false);

		VolumeActors = 0.f;
	}
	else
	{
		CB_Actors->SetCheckedState(ECheckBoxState::Unchecked);

		Slider_Actors->SetIsEnabled(true);

		VolumeEffects = Slider_Effects->GetValue();
	}
	bIsMuteActors = bIsChecked;
}

void UAudio_Settings::OnEffectsValueChange(float NewValue)
{
	Slider_Effects->SetValue(NewValue);

	VolumeEffects = NewValue;
}

void UAudio_Settings::OnEffectsMuteChange(bool bIsChecked)
{
	if (bIsChecked)
	{
		CB_Effects->SetCheckedState(ECheckBoxState::Checked);

		Slider_Effects->SetIsEnabled(false);

		VolumeEffects = 0.f;
	}
	else
	{
		CB_Effects->SetCheckedState(ECheckBoxState::Unchecked);

		Slider_Effects->SetIsEnabled(true);

		VolumeEffects = Slider_Effects->GetValue();
	}
	bIsMuteEffects = bIsChecked;
}

void UAudio_Settings::OnMuteAllChange(bool bIsChecked)
{
	if (bIsChecked)
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Checked);
		CB_Music->SetCheckedState(ECheckBoxState::Checked);
		CB_Actors->SetCheckedState(ECheckBoxState::Checked);
		CB_Effects->SetCheckedState(ECheckBoxState::Checked);

		Slider_Music->SetIsEnabled(false);
		Slider_Actors->SetIsEnabled(false);
		Slider_Effects->SetIsEnabled(false);

		VolumeMusic = 0.f;
		VolumeActors = 0.f;
		VolumeEffects = 0.f;
	}
	else
	{
		CB_MuteAll->SetCheckedState(ECheckBoxState::Unchecked);
		CB_Music->SetCheckedState(ECheckBoxState::Unchecked);
		CB_Actors->SetCheckedState(ECheckBoxState::Unchecked);
		CB_Effects->SetCheckedState(ECheckBoxState::Unchecked);

		Slider_Music->SetIsEnabled(true);
		Slider_Actors->SetIsEnabled(true);
		Slider_Effects->SetIsEnabled(true);

		VolumeMusic = Slider_Music->GetValue();
		VolumeActors = Slider_Actors->GetValue();
		VolumeEffects = Slider_Effects->GetValue();
	}
	bIsMuteAll = bIsChecked;
}

void UAudio_Settings::NativeDestruct()
{
	Super::NativeDestruct();

	Slider_Music->OnValueChanged.RemoveAll(this);
	Slider_Actors->OnValueChanged.RemoveAll(this);
	Slider_Effects->OnValueChanged.RemoveAll(this);

	CB_Music->OnCheckStateChanged.RemoveAll(this);
	CB_Actors->OnCheckStateChanged.RemoveAll(this);
	CB_Effects->OnCheckStateChanged.RemoveAll(this);
}
