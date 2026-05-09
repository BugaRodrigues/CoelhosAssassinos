#include "Widgets/Saving&Loading/Saving_E_Loading.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Components/Image.h"

void USaving_E_Loading::SetIsSaving(bool bIsSaving)
{
	if (bIsSaving)
	{
		IMG_SavingELoading->SetBrushFromTexture(SavingIcon);
	}
	else
	{
		IMG_SavingELoading->SetBrushFromTexture(LoadingIcon);
	}
}