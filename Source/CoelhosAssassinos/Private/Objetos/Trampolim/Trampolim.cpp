#include "Objetos/Trampolim/Trampolim.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"

ATrampolim::ATrampolim()
{
	PrimaryActorTick.bCanEverTick = false;

	LaunchForce = 1000.f;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->InitBoxExtent(FVector(2.f, 55.f, 32.f));
	Comp_BoxCollision->SetCollisionResponseToAllChannels(ECR_Block);
	Comp_BoxCollision->SetGenerateOverlapEvents(false);

	Comp_PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbook"));
	Comp_PaperFlipbook->SetupAttachment(RootComponent);
	Comp_PaperFlipbook->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -25.f), FRotator(0., -90., 0.));
	Comp_PaperFlipbook->Stop();
	Comp_PaperFlipbook->SetLooping(false);

	Comp_Box_JumpArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collsion (Jump Area)"));
	Comp_Box_JumpArea->SetupAttachment(RootComponent);
	Comp_Box_JumpArea->InitBoxExtent(FVector(2.f, 48.f, 10.f));
	Comp_Box_JumpArea->SetRelativeLocation(FVector(0.f, 0.f, 40.f));
	Comp_Box_JumpArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Comp_Box_JumpArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	Comp_Box_JumpArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Comp_Box_JumpArea->SetGenerateOverlapEvents(true);
	Comp_Box_JumpArea->OnComponentBeginOverlap.AddDynamic(this, &ATrampolim::OnJumpAreaOverlap);

	Comp_Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Componente Audio"));
	Comp_Audio->SetupAttachment(RootComponent);
	Comp_Audio->bAutoActivate = false;
	Comp_Audio->bOverrideAttenuation = true;
	Comp_Audio->SetSound(SB_Jumping ? SB_Jumping : nullptr);
	FSoundAttenuationSettings AttenuationSettings;
	AttenuationSettings.AttenuationShapeExtents = FVector(0.f, 600.f, 0.f);
	AttenuationSettings.FalloffDistance = 900.f;
	Comp_Audio->AdjustAttenuation(AttenuationSettings);
}

void ATrampolim::OnJumpAreaOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APaperCharacter* PaperCharacter = Cast<APaperCharacter>(OtherActor))
	{
		if (PaperCharacter->GetVelocity().Z < 0.f)
		{
			PaperCharacter->LaunchCharacter(FVector(0.f, 0.f, LaunchForce), false, true);

			Comp_PaperFlipbook->PlayFromStart();

			Comp_Audio->Play();
		}
	}
}
