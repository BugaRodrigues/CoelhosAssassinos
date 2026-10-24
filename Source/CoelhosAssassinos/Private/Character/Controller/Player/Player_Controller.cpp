#include "Character/Controller/Player/Player_Controller.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
#include "Widgets/Contador/ContadorDeVida.h"
#include "Widgets/Menu/Pause/MenuPause.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

void APlayer_Controller::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!GameInstance) return;

	Interface_SaveGame = Cast<IInterface_SaveGame>(GameInstance);
	if (!Interface_SaveGame) return;

	GameData = Interface_SaveGame->GetGameData();
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

void APlayer_Controller::AddMenuPause()
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

	(bAddorRemove) ? Subsystem->AddMappingContext(MappingContext, 0, Options) : Subsystem->RemoveMappingContext(MappingContext);
	#if PLATFORM_ANDROID || PLATFORM_IOS || WITH_EDITOR
		ActivateTouchInterface(bAddorRemove ? TouchInterface : nullptr);
	#endif
}

void APlayer_Controller::ReiniciarMapa()
{
	if (GameData->Vida <= 0)
	{
		AddOrRemoveMenuVida(false);

		GameData = NewObject<UGame_Save>();

		if (Interface_SaveGame)
		{
			Interface_SaveGame->SaveGame(true, GameData);

			TWeakObjectPtr<APlayer_Controller> WeakThis(this);
			Interface_SaveGame->OnSaveGameComplete.BindLambda([WeakThis]
			{
				if (!WeakThis.IsValid()) return;

				WeakThis->AddOrRemoveMenuVida(true);
			});
		}
	}
	else
	{
		Interface_SaveGame->SaveGame(true, GameData);

		TWeakObjectPtr<UWorld> WeakWorld(GetWorld());
		Interface_SaveGame->OnSaveGameComplete.BindLambda( [WeakWorld]
		{
			if (!WeakWorld.IsValid()) return;

			UGameplayStatics::OpenLevel(WeakWorld.Get(), WeakWorld->GetFName());
		});
	}
}

void APlayer_Controller::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(this, Sound);
}

void APlayer_Controller::SaveGame(bool bIsAsync)
{
	if (!Interface_SaveGame) return;

	Interface_SaveGame->SaveGame(bIsAsync, GameData);
}

int32 APlayer_Controller::GetVida()
{
	if (!GameData) return 0;

	return GameData->Vida;
}

void APlayer_Controller::SetVida(int32 NewVida)
{
	if (!GameData) return;

	GameData->Vida = NewVida;
}

void APlayer_Controller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::EndPlayInEditor || EndPlayReason == EEndPlayReason::Quit)
	{
		if (Interface_SaveGame) Interface_SaveGame->OnSaveGameComplete.Unbind();
	}
}
