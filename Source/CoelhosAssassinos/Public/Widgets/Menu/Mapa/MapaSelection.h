#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapaSelection.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UMapaSelection : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions
		public:
			virtual void NativeConstruct() override;

			virtual void NativeDestruct() override;

		private:
			void SetLevelsButtons();

			UFUNCTION()
			void OnClicked_BtnVoltar();

			class UGame_Save* GetGameData();

	#pragma endregion

	#pragma region Properties
		public:

		private:
			UPROPERTY(Meta = (BindWidget))
			TObjectPtr<class UButton> Btn_Voltar;

			UPROPERTY(Meta = (BindWidget))
			TObjectPtr<class UCanvasPanel> MainCanvas;

			UPROPERTY(EditAnywhere)
			TSubclassOf<class UButton_MapaSelection> Class_Btn_Mapa;

			UPROPERTY(EditAnywhere, Category = "Mapas")
			TArray<FName> Mapas;

			UPROPERTY(EditAnywhere, Category = "Mapas")
			FName Map_MainMenu;

	#pragma endregion

};
