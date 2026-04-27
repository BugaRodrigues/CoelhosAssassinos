#include "GameInstance/Main_Game_Instance.h"
#include "Kismet/GameplayStatics.h"

void UMain_Game_Instance::Init()
{
	Super::Init();

	LoadSave(false);
}

void UMain_Game_Instance::LoadSave(bool bAsync)
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		if (bAsync)
		{
			auto AsyncLoadDelegate = FAsyncLoadGameFromSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncLoadGameComplete);

			UGameplayStatics::AsyncLoadGameFromSlot(SaveSlotName, 0, AsyncLoadDelegate);
		}
		else
		{
			SaveData = Cast<UGame_Save>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		}
	}
	else 
	{
		SaveData = Cast<UGame_Save>(UGameplayStatics::CreateSaveGameObject(UGame_Save::StaticClass()));
	}
}

void UMain_Game_Instance::OnAsyncLoadGameComplete(const FString& SlotName, int32 UserIndex, USaveGame* LoadedGameData)
{
	if (LoadedGameData)
	{
		SaveData = Cast<UGame_Save>(LoadedGameData);

		OnLoadGameComplete.ExecuteIfBound();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Falha ao carregar o jogo!!!"));
	}
}

void UMain_Game_Instance::SaveGame(bool bAsync, UGame_Save* NewSaveData)
{
	if (!SaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("Não existe um jogo para ser salvo!!!"));

		return;
	}

	SaveData = NewSaveData;

	if (bAsync)
	{
		auto AsyncSaveDelegate = FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UMain_Game_Instance::OnAsyncSaveGameComplete);

		UGameplayStatics::AsyncSaveGameToSlot(SaveData, SaveSlotName, 0, AsyncSaveDelegate);
	}
	else
	{
		if (!UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0))
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
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Falha ao salvar o jogo!!!"));
	}
}

UGame_Save* UMain_Game_Instance::GetSaveData()
{
	return SaveData;
}

