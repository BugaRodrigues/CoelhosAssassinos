#include "Objetos/Porta/Porta.h"
#include "Interface/SaveGame/Interface_SaveGame.h"
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

	SaveGameInterface = Cast<IInterface_SaveGame>(GetGameInstance());

	if (!SaveGameInterface) { UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null. (Porta Init)")); return; }

	SaveData = SaveGameInterface->GetSaveData();

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
	if (!SaveGameInterface) UE_LOG(LogTemp, Error, TEXT("SaveGameInterface is null"));

	if (!bIsOpen) return;

	if (!OtherActor && OtherActor == this) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	SaveData->MapaName = FName(*NextLevel.GetAssetName());

	SaveGameInterface->SaveGame(true, SaveData);

	UGameplayStatics::OpenLevel(this, FName(*NextLevel.GetAssetName()));
}

void APorta::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GameMode)
	{
		GameMode->OpenGate.Unbind();
	}
}

void APorta::OpenDoor()
{
	bIsOpen = true;

	Comp_Sprite->SetSprite(Sprite_Open ? Sprite_Open : nullptr);
}
