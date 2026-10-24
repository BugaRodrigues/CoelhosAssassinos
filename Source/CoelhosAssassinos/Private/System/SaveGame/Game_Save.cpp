#include "System/SaveGame/Game_Save.h"

UGame_Save::UGame_Save()
{
	Vida = 3;
	Mapa_Inicial = TEXT("Map_Tutorial");
	Mapa_Atual = TEXT("None");
	Mapas_Completos = {};
}
