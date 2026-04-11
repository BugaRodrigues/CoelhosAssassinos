#include "Character/Inimigo/CoelhoZumbi.h"
#include "Materials/MaterialInstanceDynamic.h"
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

	SetRootComponent(GetCapsuleComponent());
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACoelhoZumbi::OnInimigoBeginOverlap);

	Comp_Box_Chao = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Chao"));
	Comp_Box_Chao->SetupAttachment(GetRootComponent());
	Comp_Box_Chao->OnComponentEndOverlap.AddDynamic(this, &ACoelhoZumbi::OnEndCollisionChao);

	Comp_Box_Parede = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Parede"));
	Comp_Box_Parede->SetupAttachment(GetRootComponent());
	Comp_Box_Parede->OnComponentBeginOverlap.AddDynamic(this, &ACoelhoZumbi::OnBeginCollisionParede);

	Tags.Add(FName("Inimigo"));
}

void ACoelhoZumbi::BeginPlay()
{
	Super::BeginPlay();

	Direction = GetRootComponent()->GetRelativeRotation().Yaw > 0 ? 1 : -1;

	#pragma region Create Dynamic Material
		if (GetSprite()->GetMaterial(0))
		{
			UMaterialInterface* BaseMaterial = GetSprite()->GetMaterial(0);

			if (BaseMaterial)
			{
				Mat_Inst_Dynamic = GetSprite()->CreateDynamicMaterialInstance(0, BaseMaterial);
			}
		}
	#pragma endregion

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
	if (OtherActor)
	{
		if (OtherActor->ActorHasTag("Player")) return;

		Direction *= -1;

		GetRootComponent()->SetRelativeRotation(FRotator(0.f, Direction < 0 ? -90.f : 90.f, 0.f));
	}
}

void ACoelhoZumbi::OnInimigoBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (!OtherActor->ActorHasTag("Player")) return;

		Direction *= -1;

		Cast<APaperCharacter>(OtherActor)->LaunchCharacter(FVector(0.f, -100.f * Direction, 250.f), true, true);
	}
}

float ACoelhoZumbi::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Vida -= DamageAmount;

	bCanMove = false;

	if (!Mat_Inst_Dynamic) return DamageAmount;

	Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor(.17f, .13f, 0.f));

	if (Vida <= 0.f)
	{
		bCanMove = false;

		FVector Scale = GetSprite()->GetRelativeScale3D();

		GetSprite()->SetRelativeScale3D(FVector(Scale.X, .1f, Scale.Z));

		Comp_Box_Chao->DestroyComponent();
		Comp_Box_Parede->DestroyComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorld()->GetTimerManager().SetTimer(TH_Morte, this, &ACoelhoZumbi::Morreu, 1.f);
		
		GetWorld()->GetTimerManager().SetTimer(TH_TomouDano, this, &ACoelhoZumbi::TomouDano, .075f);

		return DamageAmount;
	}

	GetWorld()->GetTimerManager().SetTimer(TH_StopMove, this, &ACoelhoZumbi::StopMove, .5f);

	GetWorld()->GetTimerManager().SetTimer(TH_TomouDano, this, &ACoelhoZumbi::TomouDano, .1f);

	return DamageAmount;
}

void ACoelhoZumbi::TomouDano() { Mat_Inst_Dynamic->SetVectorParameterValue(TEXT("Cor"), FLinearColor::White); }
