#include "Objetos/Armadilhas/ChaoFalso/ChaoFalso.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
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
	Comp_PaperSprite->SetupAttachment(RootComponent);
	if (Sprite_Ativo) Comp_PaperSprite->SetSprite(Sprite_Ativo);

	Comp_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Componente Audio"));
	Comp_Audio->SetupAttachment(RootComponent);
	Comp_Audio->bAutoActivate = false;
	Comp_Audio->bOverrideAttenuation = true;
	Comp_Audio->AttenuationOverrides.AttenuationShapeExtents.X = 300.;
	Comp_Audio->AttenuationOverrides.FalloffDistance = 500.f;
}

void AChaoFalso::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && !bIsProcessing)
	{
		if (OtherActor->GetVelocity().Z <= 0.f)
		{
			PlaySound(SB_ChaoFechar);

			bIsProcessing = true;

			GetWorld()->GetTimerManager().ClearTimer(TH_Destruct);
			GetWorld()->GetTimerManager().SetTimer(TH_Destruct, this, &AChaoFalso::Destruct, 1.5f);

			if (Sprite_Completo) Comp_PaperSprite->SetSprite(Sprite_Completo);

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
		if (Sprite_Completo) Comp_PaperSprite->SetSprite(Sprite_Completo);

		Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void AChaoFalso::Destruct()
{
	PlaySound(SB_ChaoQuebrar);

	if (Sprite_Inativo) Comp_PaperSprite->SetSprite(Sprite_Inativo);

	GetWorld()->GetTimerManager().ClearTimer(TH_Construct);
	GetWorld()->GetTimerManager().SetTimer(TH_Construct, this, &AChaoFalso::Construct, 3.f);

	Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AChaoFalso::Construct()
{
	PlaySound(SB_ChaoRegenerar);

	bIsProcessing = false;

	if (Sprite_Ativo) Comp_PaperSprite->SetSprite(Sprite_Ativo);

	Comp_BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AChaoFalso::PlaySound(USoundBase* Sound)
{
	if (!Sound || !Comp_Audio) return;

	Comp_Audio->SetSound(Sound);
	Comp_Audio->Play();
}
