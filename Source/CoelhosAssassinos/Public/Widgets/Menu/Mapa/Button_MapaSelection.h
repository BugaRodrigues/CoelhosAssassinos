#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Button_MapaSelection.generated.h"

UENUM()
enum class ELevelState : uint8
{
	Completo	UMETA(DisplayName = "Completo"),
	Atual		UMETA(DisplayName = "Atual"),
	Bloqueado	UMETA(DisplayName = "Bloqueado")
};

UCLASS()
class COELHOSASSASSINOS_API UButton_MapaSelection : public UUserWidget
{
	GENERATED_BODY()
		
	#pragma region Functions
		public:
			virtual void NativeConstruct() override;

			virtual void NativeDestruct() override;

		private:
			UFUNCTION()
			void OnClickMapa();

	#pragma endregion

	#pragma region Properties
		public:
			UPROPERTY()
			FName Mapa;

			UPROPERTY()
			FText TextIndice;

			UPROPERTY()
			ELevelState StateLevel;

		private:
			UPROPERTY(Meta = (BindWidget))
			TObjectPtr<class UButton> Btn_Mapa;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<class UTextBlock> Tb_MapIndice;

	#pragma endregion

};
