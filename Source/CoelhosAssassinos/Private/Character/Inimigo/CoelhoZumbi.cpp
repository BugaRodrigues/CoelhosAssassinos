#include "Character/Inimigo/CoelhoZumbi.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h" 
#include "PaperFlipbookComponent.h"
#include "TimerManager.h"

ACoelhoZumbi::ACoelhoZumbi()
{
	PrimaryActorTick.bCanEverTick = true;

	Vida = 20.f;
	Velocidade = 300.f;
	bCanMove = true;

	Comp_Box_Chao = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Chao"));
	Comp_Box_Chao->SetupAttachment(GetRootComponent());
	Comp_Box_Chao->OnComponentEndOverlap.AddDynamic(this, &ACoelhoZumbi::OnEndCollisionChao);

	Comp_Box_Parede = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Parede"));
	Comp_Box_Parede->SetupAttachment(GetRootComponent());
	Comp_Box_Parede->OnComponentBeginOverlap.AddDynamic(this, &ACoelhoZumbi::OnBeginCollisionParede);
}

void ACoelhoZumbi::BeginPlay()
{
	Super::BeginPlay();

	Direction = GetRootComponent()->GetRelativeRotation().Yaw > 0 ? 1 : -1;
}

void ACoelhoZumbi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanMove)
	{
		AddMovementInput(FVector(0.f, Velocidade * DeltaTime, 0.f), Direction);
	}
}

void ACoelhoZumbi::OnEndCollisionChao(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (!OtherActor->ActorHasTag("Mapa")) return;

		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void ACoelhoZumbi::OnBeginCollisionParede(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Ator: %s, Component: %s"), *OtherActor->GetClass()->GetName(), *OtherComp->GetClass()->GetName()));

	if (OtherActor)
	{
		if (OtherActor->ActorHasTag("Player")) return;

		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

float ACoelhoZumbi::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Vida -= DamageAmount;

	bCanMove = false;

	GetSprite()->SetSpriteColor(FColor::Red);

	if (Vida <= 0.f)
	{
		bCanMove = false;

		FVector Scale = GetSprite()->GetRelativeScale3D();

		GetSprite()->SetRelativeScale3D(FVector(Scale.X, .1f, Scale.Z));

		Comp_Box_Chao->DestroyComponent();
		Comp_Box_Parede->DestroyComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		return 0.f;
	}

	GetWorld()->GetTimerManager().SetTimer(TH_StopMove, this, &ACoelhoZumbi::StopMove, .5f);

	GetWorld()->GetTimerManager().SetTimer(TH_TomouDano, this, &ACoelhoZumbi::TomouDano, .05f);

	return DamageAmount;
}

void ACoelhoZumbi::TomouDano() { GetSprite()->SetSpriteColor(FColor::White); }
