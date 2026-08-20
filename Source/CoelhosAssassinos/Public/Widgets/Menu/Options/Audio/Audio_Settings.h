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
			TObjectPtr<class USlider> Slider_Actors;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_Actors;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class USlider> Slider_Effects;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_Effects;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UCheckBox> CB_MuteAll;

			bool bIsMuteMusic = false;
			bool bIsMuteActors = false;
			bool bIsMuteEffects = false;
			bool bIsMuteAll = false;

			float VolumeMusic = 100.f;
			float VolumeActors = 100.f;
			float VolumeEffects = 100.f;

	#pragma endregion
};
