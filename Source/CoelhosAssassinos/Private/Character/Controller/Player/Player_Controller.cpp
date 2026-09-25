#include "Character/Controller/Player/Player_Controller.h"
#include "GameInstance/Main_Game_Instance.h"
#include "Widgets/Contador/ContadorDeVida.h"
#include "Widgets/Menu/Pause/MenuPause.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	GetSaveGame();
	AddOrRemoveImc(true);
	AddOrRemoveMenuVida(true);
}

void APlayer_Controller::AddOrRemoveMenuVida(bool bAddOrRemove)
{
	if (!ContadorVidaClass) return;

	if (!ContadorVida) ContadorVida = CreateWidget<UContadorDeVida>(this, ContadorVidaClass);

	if (bAddOrRemove)
	{
		ContadorVida->AddToViewport();
		ContadorVida->SetVida(GameData->Vida);
	}
	else
	{
		ContadorVida->RemoveFromParent();
	}
}

void APlayer_Controller::UpdateVida(int32 NewVida)
{
	if (!ContadorVida) return;

	ContadorVida->SetVida(NewVida);
}

void APlayer_Controller::AddOrRemoveMenuPause()
{
	if (!MenuPauseClass) return;

	MenuPause = CreateWidget<UMenuPause>(this, MenuPauseClass);
	if (!MenuPause) return;

	MenuPause->AddToViewport();
}

void APlayer_Controller::AddOrRemoveImc(bool bAddorRemove)
{
	if (!MappingContext && !TouchInterface) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!Subsystem) return;

	FModifyContextOptions Options;
	Options.bNotifyUserSettings = true;

	ActivateTouchInterface(bAddorRemove ? TouchInterface : nullptr);
	(bAddorRemove) ? Subsystem->AddMappingContext(MappingContext, 0, Options) : Subsystem->RemoveMappingContext(MappingContext);
}

void APlayer_Controller::ReiniciarMapa()
{
	if (GameData->Vida <= 0)
	{
		AddOrRemoveMenuVida(false);

		GameData = NewObject<UGame_Save>();

		if (GameInstance)
		{
			GameInstance->SaveGame(true, GameData);

			TWeakObjectPtr<APlayer_Controller> WeakThis(this);
			GameInstance->OnSaveGameComplete.BindLambda([WeakThis]
			{
				if (!WeakThis.IsValid()) return;

				WeakThis->AddOrRemoveMenuVida(true);
			});
		}
	}
	else
	{
		GameInstance->SaveGame(true, GameData);
	}
	AddOrRemoveMenuPause();
}

void APlayer_Controller::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(this, Sound);
}

void APlayer_Controller::SaveGame(bool bIsAsync)
{
	if (!GameInstance) return;

	GameInstance->SaveGame(bIsAsync, GameData);
}

int32 APlayer_Controller::GetVida()
{
	if (!GameData) return 0;

	return GameData->Vida;
}

bool APlayer_Controller::GetSaveGame()
{
	GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());
	if (!GameInstance) return false;

	GameData = GameInstance->GetGameData();

	return true;
}

void APlayer_Controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::EndPlayInEditor || EndPlayReason == EEndPlayReason::Quit)
	{
		if (GameInstance) GameInstance->OnSaveGameComplete.Unbind();
	}
}
