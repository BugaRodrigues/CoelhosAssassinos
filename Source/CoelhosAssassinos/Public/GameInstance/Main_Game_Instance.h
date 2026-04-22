#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Main_Game_Instance.generated.h"

class COELHOSASSASSINOS_API Main_Game_Instance : public UGameInstance, public IInterface_SaveGame
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			UFUNCTION()
			virtual void OnSaveGame() override;

			UFUNCTION()
			virtual void OnLoadGame() override;

	#pragma endregion

	#pragma region Variables

		public:

		private:
			const FString SaveSlotName = "MainSaveSlot";

	#pragma endregion
};
