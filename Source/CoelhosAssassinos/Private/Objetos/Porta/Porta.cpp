#include "Objetos/Porta/Porta.h"
#include "GameInstance/Main_Game_Instance.h"
#include "GameFramework/GameModeBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/MainGameMode.h"
#include "PaperSpriteComponent.h"

APorta::APorta()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsOpen = false;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Comp_BoxCollision"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &APorta::OnBeginOverlap);

	Comp_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Comp_Sprite"));
	Comp_Sprite->SetupAttachment(RootComponent);
	Comp_Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp_Sprite->SetGenerateOverlapEvents(false);
	Comp_Sprite->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void APorta::BeginPlay()
{
	Super::BeginPlay();

	PlaySound(SB_BaseFechado);

	#pragma region Get Save Game
		GameInstance = Cast<UMain_Game_Instance>(GetGameInstance());

		if (IsValid(GameInstance))
		{
			SaveGameInterface = Cast<IInterface_SaveGame>(GameInstance);
		}

		if (!SaveGameInterface) { UE_LOG(LogTemp, Error, TEXT("SaveGameInterface or GameInstance is null. (Porta Init)")); return; }

		SaveData = SaveGameInterface->GetSaveData();
	#pragma endregion

	Comp_Sprite->SetSprite(Sprite_Closed ? Sprite_Closed : nullptr);

	#pragma region Get GameMode and Bind Delegate
		GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode)
		{
			GameMode->OpenGate.BindUObject(this, &APorta::OpenDoor);
		}
	#pragma endregion
}

void APorta::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlaySound(SB_Entrar);

	if (!SaveGameInterface) UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null"));

	if (!bIsOpen) return;

	if (!OtherActor && OtherActor == this) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	SaveData->MapaName = FName(*NextLevel.GetAssetName());

	if (IsValid(GameInstance))
	{
		SaveGameInterface->SaveGame(true, SaveData);

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

	Comp_Sprite->SetSprite(Sprite_Open ? Sprite_Open : nullptr);

	GetWorldTimerManager().SetTimerForNextTick([this] () { PlaySound(SB_BaseAberto); });
}

void APorta::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
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
