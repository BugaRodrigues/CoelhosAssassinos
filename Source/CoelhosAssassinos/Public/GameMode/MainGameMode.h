#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_DELEGATE_TwoParams(FShowKeys, int32 /*TotalChaves*/, int32 /*ChavesColetadas*/)
DECLARE_DELEGATE(FOpenGate)

UCLASS()
class COELHOSASSASSINOS_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			AMainGameMode();

		protected:
			virtual void BeginPlay() override;

			int32 GetAllKeys();

			void AddWidget();

			void OnKeyCollected() { ChavesColetadas++; ShowKeys.ExecuteIfBound(ChavesTotais, ChavesColetadas); if (ChavesColetadas >= ChavesTotais) OpenGate.ExecuteIfBound(); }

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	#pragma endregion
	
	#pragma region Variables

		public:	
			FShowKeys ShowKeys;

			FOpenGate OpenGate;

		protected:
			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<UUserWidget> HUD_WidgetClass;

			UPROPERTY()
			class UUserWidget* HUD_Widget;

			int32 ChavesTotais;

			int32 ChavesColetadas;

			class AChave* Chave;

	#pragma endregion

};
