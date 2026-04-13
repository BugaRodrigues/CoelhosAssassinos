#include "Character/Inimigo/InimigoPulador.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"

AInimigoPulador::AInimigoPulador()
{
	PrimaryActorTick.bCanEverTick = false;

	Velocidade = 300.f;
	ForcaDoPulo = 250.f;
	Direction = 1;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnBeginOverlap);

	Comp_ParedeCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Parede"));
	Comp_ParedeCollision->SetupAttachment(GetRootComponent());
	Comp_ParedeCollision->OnComponentBeginOverlap.AddDynamic(this, &AInimigoPulador::OnParedeBeginOverlap);

	Comp_ChaoCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Chao"));
	Comp_ChaoCollision->SetupAttachment(GetRootComponent());
	Comp_ChaoCollision->OnComponentEndOverlap.AddDynamic(this, &AInimigoPulador::OnChaoEndOverlap);

	Tags.Add("Inimigo");
}

void AInimigoPulador::BeginPlay()
{
	Super::BeginPlay();
	
	GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));

	Comp_ChaoCollision->SetRelativeLocation(FVector(ForcaDoPulo, 0.f + 10.f, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 10.f));
}

void AInimigoPulador::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("Player"))
		{
			AInimigoPulador::LaunchCharacter(FVector(0.f, 0.f, ForcaDoPulo), false, true);
		}
		else if (OtherActor->ActorHasTag("Mapa"))
		{
			if (GetWorldTimerManager().IsTimerActive(TH_Jump)) return;

			GetWorldTimerManager().SetTimer(TH_Jump, this, &AInimigoPulador::Jump, 0.5f, false);
		}
	}
}

void AInimigoPulador::OnParedeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void AInimigoPulador::OnChaoEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (GetWorldTimerManager().IsTimerActive(TH_Jump)) GetWorldTimerManager().ClearTimer(TH_Jump);

		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void AInimigoPulador::Jump()
{
	AInimigoPulador::LaunchCharacter(FVector(0.f, Direction * ForcaDoPulo, ForcaDoPulo), true, true);
}
