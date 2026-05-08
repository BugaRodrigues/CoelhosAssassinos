#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContadorDeVida.generated.h"

UCLASS()
class COELHOSASSASSINOS_API UContadorDeVida : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			void SetVida(int32 Vida);

		protected:

	#pragma endregion

	#pragma region Variables

		public:

		protected:
			UPROPERTY(meta = (BindWidget))
			class UTextBlock* Text_Vida;

	#pragma endregion
};
