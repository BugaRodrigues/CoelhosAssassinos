#include "Objetos/Chave/Chave.h"
#include "Components/BoxComponent.h"
#include "Kismet\GameplayStatics.h"
#include "PaperSpriteComponent.h"

AChave::AChave()
{
	PrimaryActorTick.bCanEverTick = false;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Comp_BoxCollision"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AChave::OnBeginOverlap);

	Comp_Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Comp_Sprite"));
	Comp_Sprite->SetupAttachment(RootComponent);
	Comp_Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp_Sprite->SetGenerateOverlapEvents(false);
	Comp_Sprite->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
}

void AChave::BeginPlay()
{
	Super::BeginPlay();
	
}

void AChave::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag(FName("Player")))
		{
			if (SB_AoPegar) UGameplayStatics::PlaySound2D(GetWorld(), SB_AoPegar);

			OnKeyCollected.ExecuteIfBound();

			Destroy();
		}
	}
}
