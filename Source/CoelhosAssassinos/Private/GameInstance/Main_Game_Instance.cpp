#include "GameInstance/Main_Game_Instance.h"
#include "Widgets/Saving&Loading/Saving_E_Loading.h"
#include "Kismet/GameplayStatics.h"

void UMain_Game_Instance::Init()
{
	Super::Init();

	LoadSave(false);

	LoadSettings(false);
}

#pragma region Save Game

	void UMain_Game_Instance::LoadSave(bool bAsync)
	{
		if (UGameplayStatics::DoesSaveGameExist(SaveGameSlot, 0))
		{
			if (bAsync)
			{
				AddSavingAndLoadingWidget(false);

				auto AsyncLoadDelegate = FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncLoadGameComplete);

				UGameplayStatics::AsyncLoadGameFromSlot(SaveGameSlot, 0, AsyncLoadDelegate);
			}
			else
			{
				GameData = Cast<UGame_Save>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
			}
		}
		else 
		{
			GameData = Cast<UGame_Save>(UGameplayStatics::CreateSaveGameObject(UGame_Save::StaticClass()));
		}
	}

	void UMain_Game_Instance::OnAsyncLoadGameComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedGameData)
	{
		if (LoadedGameData)
		{
			GameData = Cast<UGame_Save>(LoadedGameData);

			OnLoadGameComplete.ExecuteIfBound();

			if (SavingAndLoading_Widget) SavingAndLoading_Widget->RemoveFromParent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Falha ao carregar o jogo!!!"));
		}
	}

	void UMain_Game_Instance::SaveGame(bool bAsync, UGame_Save* NewGameData)
	{
		if (!GameData)
		{
			UE_LOG(LogTemp, Error, TEXT("Não existe um jogo para ser salvo!!!"));

			return;
		}

		GameData = NewGameData;

		if (bAsync)
		{
			AddSavingAndLoadingWidget(true);

			auto AsyncSaveDelegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncSaveGameComplete);

			UGameplayStatics::AsyncSaveGameToSlot(GameData, SaveGameSlot, 0, AsyncSaveDelegate);
		}
		else
		{
			if (!UGameplayStatics::SaveGameToSlot(GameData, SaveGameSlot, 0))
			{
				UE_LOG(LogTemp, Error, TEXT("Falha ao salvar o jogo!!!"));
			}
		}
	}

	void UMain_Game_Instance::OnAsyncSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
	{
		if (bSuccess)
		{
			OnSaveGameComplete.ExecuteIfBound();

			if (SavingAndLoading_Widget) SavingAndLoading_Widget->RemoveFromParent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Falha ao salvar o jogo!!!"));
		}
	}

#pragma endregion

#pragma region Save Settings

	void UMain_Game_Instance::LoadSettings(bool bAsync)
	{
		if (UGameplayStatics::DoesSaveGameExist(SaveSettingsSlot, 0))
		{
			if (bAsync)
			{
				AddSavingAndLoadingWidget(false);

				auto AsyncLoadDelegate = FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncLoadSettingsComplete);

				UGameplayStatics::AsyncLoadGameFromSlot(SaveSettingsSlot, 0, AsyncLoadDelegate);
			}
			else
			{
				SettingsData = Cast<USettings_Save>(UGameplayStatics::LoadGameFromSlot(SaveSettingsSlot, 0));
			}
		}
		else
		{
			SettingsData = Cast<USettings_Save>(UGameplayStatics::CreateSaveGameObject(USettings_Save::StaticClass()));
		}
	}

	void UMain_Game_Instance::OnAsyncLoadSettingsComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedSettings)
	{
		if (LoadedSettings)
		{
			SettingsData = Cast<USettings_Save>(LoadedSettings);

			OnLoadSettingsComplete.ExecuteIfBound();

			if (SavingAndLoading_Widget) SavingAndLoading_Widget->RemoveFromParent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Falha ao carregar o settings!!!"));
		}
	}

	void UMain_Game_Instance::SaveSettings(bool bAsync, USettings_Save* NewSaveSettings)
	{
		if (!SettingsData)
		{
			UE_LOG(LogTemp, Error, TEXT("Não existe uma settings para ser salvo!!!"));

			return;
		}

		SettingsData = NewSaveSettings;

		if (bAsync)
		{
			AddSavingAndLoadingWidget(true);

			auto AsyncSaveDelegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncSaveSettingsComplete);

			UGameplayStatics::AsyncSaveGameToSlot(SettingsData, SaveSettingsSlot, 0, AsyncSaveDelegate);
		}
		else
		{
			if (!UGameplayStatics::SaveGameToSlot(SettingsData, SaveSettingsSlot, 0))
			{
				UE_LOG(LogTemp, Error, TEXT("Falha ao salvar o jogo!!!"));
			}
		}
	} 

	void UMain_Game_Instance::OnAsyncSaveSettingsComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
	{
		if (bSuccess)
		{
			OnSaveSettingsComplete.ExecuteIfBound();

			if (SavingAndLoading_Widget) SavingAndLoading_Widget->RemoveFromParent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Falha ao salvar a settings!!!"));
		}
	}

#pragma endregion

void UMain_Game_Instance::AddSavingAndLoadingWidget(bool bIsSaving)
{
	if (SavingAndLoadin_WidgetClass)
	{
		SavingAndLoading_Widget = CreateWidget<USaving_E_Loading>(this, SavingAndLoadin_WidgetClass);

		if (SavingAndLoading_Widget)
		{
			SavingAndLoading_Widget->AddToViewport();

			SavingAndLoading_Widget->SetIsSaving(bIsSaving);
		}
	}
}
