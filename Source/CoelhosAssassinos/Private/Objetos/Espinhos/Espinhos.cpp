#include "Objetos/Espinhos/Espinhos.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "PaperSpriteComponent.h"

AEspinhos::AEspinhos()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Damage = 10.f;

	Comp_BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision Component"));
	SetRootComponent(Comp_BoxCollision);
	Comp_BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AEspinhos::OnBeginOverlap);

	Comp_PaperSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite Component"));
	Comp_PaperSprite->SetupAttachment(RootComponent);
}

void AEspinhos::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEspinhos::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECC_PhysicsBody) return;

	if (ACharacter* Character = Cast<ACharacter>(OtherActor))
	{
		Character->LaunchCharacter(FVector(0.f, 0.f, 500.f), false, true);

		UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, UDamageType::StaticClass());
	}
}
