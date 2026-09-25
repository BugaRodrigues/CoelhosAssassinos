#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player_Controller.generated.h"

UCLASS()
class COELHOSASSASSINOS_API APlayer_Controller : public APlayerController
{
	GENERATED_BODY()
	
	#pragma region Functions
		public:
			void AddOrRemoveMenuVida(bool bAddOrRemove);

			void UpdateVida(int32 NewVida);

			void AddOrRemoveMenuPause();

			void AddOrRemoveImc(bool bAddorRemove);

			void PlaySound(USoundBase* Sound);

			void ReiniciarMapa();

			void SaveGame(bool bIsAsync);

			int32 GetVida();

		protected:
			virtual void BeginPlay() override;

			bool GetSaveGame();

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	#pragma endregion
	
	#pragma region Variaveis
		public:

		protected:
			UPROPERTY(EditAnywhere, Category = "Mapping Input")
			TObjectPtr<class UInputMappingContext> MappingContext;

			UPROPERTY(EditAnywhere, Category = "Input Touch")
			TObjectPtr<class UTouchInterface> TouchInterface;

			UPROPERTY()
			TObjectPtr<class UGame_Save> GameData;

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UContadorDeVida> ContadorVidaClass;

			UPROPERTY()
			TObjectPtr<class UContadorDeVida> ContadorVida;

			UPROPERTY(EditAnywhere, Category = "Widgets")
			TSubclassOf<class UMenuPause> MenuPauseClass;

			UPROPERTY()
			TObjectPtr<class UMenuPause> MenuPause;

			UPROPERTY()
			TObjectPtr<class UMain_Game_Instance> GameInstance;

	#pragma endregion
	
};
