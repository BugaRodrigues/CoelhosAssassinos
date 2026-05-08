#include "Widgets/Contador/ContadorDeVida.h"
#include "Components/TextBlock.h"

void UContadorDeVida::SetVida(int32 Vida)
{
	if (Text_Vida)
	{
		Text_Vida->SetText(FText::FromString(FString::Printf(TEXT("%d/3"), Vida)));
	}
}