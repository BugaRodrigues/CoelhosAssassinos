#include "Objetos/Porta/Porta.h"
#include "GameInstance/Main_Game_Instance.h"
#include "GameFramework/GameModeBase.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/MainGameMode.h"
#include "PaperSpriteComponent.h"

APorta::APorta()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsOpen = false;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Componente BoxCollision"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APorta::OnBeginOverlap);

	Comp_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Componente Sprite"));
	Comp_Sprite->SetupAttachment(RootComponent);
	Comp_Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp_Sprite->SetGenerateOverlapEvents(false);
	Comp_Sprite->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	Comp_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Componente Audio"));
	Comp_Audio->SetupAttachment(RootComponent);
	Comp_Audio->bAutoActivate = false;
	Comp_Audio->bOverrideAttenuation = true;
}

void APorta::BeginPlay()
{
	Super::BeginPlay();

	PlaySound(SB_BaseFechado);

	GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());
	if (!GameInstance) return;

	GameData = GameInstance->GetGameData();

	GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	GameMode->OpenGate.BindUObject(this, &APorta::OpenDoor);

	if (Sprite_Closed) Comp_Sprite->SetSprite(Sprite_Closed);
}

void APorta::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlaySound(SB_Entrar);

	if (!GameInstance) UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null"));

	if (!bIsOpen) return;

	if (!OtherActor && OtherActor == this) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	GameData->MapaName = FName(*NextLevel.GetAssetName());

	if (!GameInstance)
	{
		GameInstance->SaveGame(true, GameData);

		GameInstance->OnSaveGameComplete.BindLambda([this]()
		{
			UGameplayStatics::OpenLevel(this, FName(*NextLevel.GetAssetName()));
		});
	}
}

void APorta::OpenDoor()
{
	PlaySound(SB_Abriu);

	bIsOpen = true;

	if (Sprite_Open) Comp_Sprite->SetSprite(Sprite_Open);

	GetWorldTimerManager().SetTimerForNextTick([this] () { PlaySound(SB_BaseFechado); });
}

void APorta::PlaySound(USoundBase* Sound)
{
	if (!Sound || !Comp_Audio) return;

	Comp_Audio->SetSound(Sound);
	Comp_Audio->Play();
}


void APorta::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Quit)
	{
		if (GameMode)
		{
			GameMode->OpenGate.Unbind();
		}
	}
}
