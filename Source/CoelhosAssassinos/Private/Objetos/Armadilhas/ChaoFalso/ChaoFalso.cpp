#include "Objetos/Armadilhas/ChaoFalso/ChaoFalso.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "TimerManager.h"

AChaoFalso::AChaoFalso()
{
	PrimaryActorTick.bCanEverTick = false;

	bIsProcessing = false;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AChaoFalso::OnBeginOverlap);
	Comp_BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AChaoFalso::OnEndOverlap);

	Comp_PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Paper Sprite"));
	Comp_PaperSprite->SetupAttachment(GetRootComponent());
	Comp_PaperSprite->SetSprite(Sprites.IsValidIndex(1) ? Sprites[1] : nullptr);
}

void AChaoFalso::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !bIsProcessing)
	{
		if (OtherActor->GetVelocity().Z <= 0.f)
		{
			bIsProcessing = true;

			GetWorld()->GetTimerManager().ClearTimer(TH_Destruct);
			GetWorld()->GetTimerManager().SetTimer(TH_Destruct, this, &AChaoFalso::Destruct, 1.5f);

			Comp_PaperSprite->SetSprite(Sprites.IsValidIndex(2) ? Sprites[2] : nullptr);

			Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
		}
		else
		{
			Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		}
	}
}

void AChaoFalso::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && !bIsProcessing)
	{
		Comp_PaperSprite->SetSprite(Sprites.IsValidIndex(2) ? Sprites[2] : nullptr);

		Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AChaoFalso::Destruct()
{
	Comp_PaperSprite->SetSprite(Sprites.IsValidIndex(0) ? Sprites[0] : nullptr);

	GetWorld()->GetTimerManager().ClearTimer(TH_Construct);
	GetWorld()->GetTimerManager().SetTimer(TH_Construct, this, &AChaoFalso::Construct, 3.f);

	Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AChaoFalso::Construct()
{
	bIsProcessing = false;

	Comp_PaperSprite->SetSprite(Sprites.IsValidIndex(1) ? Sprites[1] : nullptr);

	Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
