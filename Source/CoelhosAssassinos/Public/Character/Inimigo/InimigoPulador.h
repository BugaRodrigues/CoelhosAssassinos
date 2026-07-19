#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "InimigoPulador.generated.h"

UENUM()
enum class ETargetJump : uint8
{
	Mapa,
	Player
};

USTRUCT()
struct FEnemyAnimations
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* Jump;

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* Falling;
};

UCLASS()
class COELHOSASSASSINOS_API AInimigoPulador : public APaperCharacter
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			AInimigoPulador();

			//virtual void Tick(float DeltaTime) override;

			//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

			UFUNCTION()
			void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			UFUNCTION()
			void OnParedeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		protected:
			virtual void BeginPlay() override;

			virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

			void Jump();

			virtual void Landed(const FHitResult& Hit) override;

			bool VerificarSeTemChao();

			FRotator SetDirectionRotation(int32 NewDirection);

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_ParedeCollision;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Velocidade;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float ForcaDoPulo;

			UPROPERTY(EditAnywhere, Category = "Animations")
			struct FEnemyAnimations Animations;

			int32 Direction;

			struct FTimerHandle TH_Jump;
			struct FTimerHandle TH_AnimJump;

			ETargetJump Target;

			FVector StartLocation;
			FVector EndLocation;
			FVector BoxExtent;

			struct FCollisionQueryParams CollisionParams;
			FQuat BoxRotation;
			struct FCollisionShape CollisionShape;
			struct FHitResult HitResult;

	#pragma endregion
	
};
