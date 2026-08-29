#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Interface/SaveGame/Interface_SaveSettings.h"

#include "Main_Game_Instance.generated.h"

DECLARE_DELEGATE(FOnLoadGameComplete)
DECLARE_DELEGATE(FOnSaveGameComplete)

DECLARE_DELEGATE(FOnLoadSettingsComplete)
DECLARE_DELEGATE(FOnSaveSettingsComplete)

UCLASS()
class COELHOSASSASSINOS_API UMain_Game_Instance : public UGameInstance, public IInterface_SaveGame, public IInterface_SaveSettings
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			virtual void Init() override;

			virtual void SaveGame(bool bAsync, UGame_Save* NewGameData) override;

			virtual void LoadSave(bool bAsync) override;

			virtual UGame_Save* GetGameData() override { return GameData; };

			virtual void SaveSettings(bool bAsync, USettings_Save* NewSettingsData) override;

			virtual void LoadSettings(bool bAsync) override;

			virtual USettings_Save* GetSettingsData() override { return SettingsData; };

		private:
			void OnAsyncSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

			void OnAsyncSaveSettingsComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

			void OnAsyncLoadGameComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedGameData);

			void OnAsyncLoadSettingsComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedSettings);

			void AddSavingAndLoadingWidget(bool bIsSaving);

	#pragma endregion

	#pragma region Variables

		public:
			FOnLoadGameComplete OnLoadGameComplete;
			FOnSaveGameComplete OnSaveGameComplete;

			FOnLoadSettingsComplete OnLoadSettingsComplete;
			FOnSaveSettingsComplete OnSaveSettingsComplete;

		private:
			const FString SaveGameSlot = TEXT("MainGameSave");
			const FString SaveSettingsSlot = TEXT("MainSettingsSave");

			UPROPERTY()
			TObjectPtr<UGame_Save> GameData;

			UPROPERTY()
			TObjectPtr<USettings_Save> SettingsData;

			UPROPERTY(EditDefaultsOnly, Category = "Widget")
			TSubclassOf<class USaving_E_Loading> SavingAndLoadin_WidgetClass;

			UPROPERTY()
			TObjectPtr<class USaving_E_Loading> SavingAndLoading_Widget;

	#pragma endregion
};
