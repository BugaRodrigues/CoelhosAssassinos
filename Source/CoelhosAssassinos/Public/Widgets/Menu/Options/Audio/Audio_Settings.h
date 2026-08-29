#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Audio_Settings.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UAudio_Settings : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions

		public:

		protected:
			virtual void NativeConstruct() override;

			UFUNCTION()
			void OnMusicValueChange(float NewValue);

			UFUNCTION()
			void OnMusicMuteChange(bool bIsChecked);

			UFUNCTION()
			void OnActorsValueChange(float NewValue);

			UFUNCTION()
			void OnActorsMuteChange(bool bIsChecked);

			UFUNCTION()
			void OnEffectsValueChange(float NewValue);

			UFUNCTION()
			void OnEffectsMuteChange(bool bIsChecked);

			UFUNCTION()
			void OnMuteAllChange(bool bIsChecked);

			void ActiveDesactveSound(class USlider* Slider, class UTextBlock* Text, class UCheckBox* Check, bool bIsOn);

			virtual void NativeDestruct() override;

	#pragma endregion
	
	#pragma region Variaveis

		public:

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class USlider> Slider_Music;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_Music;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UTextBlock> TB_MusicVolume;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class USlider> Slider_Actors;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_Actors;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UTextBlock> TB_ActorsVolume;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class USlider> Slider_Effects;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_Effects;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UTextBlock> TB_EffectsVolume;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_MuteAll;

			UPROPERTY(EditAnywhere, Category = "Sound | Mix")
			TObjectPtr<class USoundMix> SM_Target;

			UPROPERTY(EditAnywhere, Category = "Sound | Class")
			TObjectPtr<class USoundClass> SC_Music;

			UPROPERTY(EditAnywhere, Category = "Sound | Class")
			TObjectPtr<class USoundClass> SC_Actors;

			UPROPERTY(EditAnywhere, Category = "Sound | Class")
			TObjectPtr<class USoundClass> SC_Effects;

			UPROPERTY()
			TObjectPtr<class USettings_Save> SettingsData;

			TObjectPtr<class UMain_Game_Instance> GameInstance;

			TObjectPtr<class IInterface_SaveSettings> Interface_SaveSettings;

	#pragma endregion
};
