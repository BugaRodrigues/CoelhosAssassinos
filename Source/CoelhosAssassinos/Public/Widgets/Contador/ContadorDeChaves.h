#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContadorDeChaves.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UContadorDeChaves : public UUserWidget
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	

		protected:
			virtual void NativeConstruct() override;

			virtual void NativeDestruct() override;

			UFUNCTION()
			void UpdateContador(int32 TotalChaves, int32 ChavesColetadas);

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(meta = (BindWidget))
			class UTextBlock* Text_Contador;

			class AMainGameMode* GameMode;

	#pragma endregion
	
};
