#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Main_Game_Instance.generated.h"

DECLARE_DELEGATE(FOnLoadGameComplete)
DECLARE_DELEGATE(FOnSaveGameComplete)

UCLASS()
class COELHOSASSASSINOS_API UMain_Game_Instance : public UGameInstance, public IInterface_SaveGame
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			virtual void Init() override;

		private:
			virtual void SaveGame(bool bAsync, UGame_Save* NewSaveData) override;

			virtual void LoadSave(bool bAsync) override;

			virtual UGame_Save* GetSaveData() override { return SaveData; };

			void OnAsyncSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);

			void OnAsyncLoadGameComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedGameData);

			void AddSavingAndLoadingWidget(bool bIsSaving);

	#pragma endregion

	#pragma region Variables

		public:
			FOnLoadGameComplete OnLoadGameComplete;
			FOnSaveGameComplete OnSaveGameComplete;

		private:
			const FString SaveSlotName = TEXT("MainSaveSlot");

			UPROPERTY()
			UGame_Save* SaveData;

			UPROPERTY(EditDefaultsOnly, Category = "Widget")
			TSubclassOf<class USaving_E_Loading> SavingAndLoadin_WidgetClass;

			UPROPERTY()
			class USaving_E_Loading* SavingAndLoading_Widget;

	#pragma endregion
};
