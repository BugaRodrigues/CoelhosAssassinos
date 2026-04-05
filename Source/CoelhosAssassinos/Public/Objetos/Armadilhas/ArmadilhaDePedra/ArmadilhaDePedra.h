
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmadilhaDePedra.generated.h"

UENUM()
enum class EFallingState : uint8
{
	Idle,
	Falling,
	Returning
};

UCLASS()
class COELHOSASSASSINOS_API AArmadilhaDePedra : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			AArmadilhaDePedra();

			//virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnPlayerPassou(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			void SetProximidade();

			void StartFalling();

			void StartReturning();

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class USceneComponent* Comp_Scene;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere)
			class UPaperSpriteComponent* Comp_PaperSprite;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxProximadade;

			UPROPERTY(EditAnywhere, Category = "Falling")
			float FallingSpeed;

			UPROPERTY(EditAnywhere, Category = "Falling")
			float ReturnSpeed;

			UPROPERTY(EditAnywhere, Category = "Falling")
			float MaxFallDistance;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Damage;

			FVector StartLocation;
			FVector Comp_StatrLocation;
			FVector EndLocation;
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			EFallingState CurrentState;

			struct FTimerHandle TH_Falling;
			struct FTimerHandle TH_Returning;

	#pragma endregion

};
