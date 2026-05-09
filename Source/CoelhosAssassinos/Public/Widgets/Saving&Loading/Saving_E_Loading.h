#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Saving_E_Loading.generated.h"

UCLASS()
class COELHOSASSASSINOS_API USaving_E_Loading : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions
		
		public:
			void SetIsSaving(bool bIsSaving);
		
		private:


	#pragma endregion

	#pragma region Variables

		public:
		
		private:
			UPROPERTY(meta = (BindWidget))
			class UImage* IMG_SavingELoading;

			UPROPERTY(EditAnywhere)
			UTexture2D* SavingIcon;

			UPROPERTY(EditAnywhere)
			UTexture2D* LoadingIcon;

	#pragma endregion
};