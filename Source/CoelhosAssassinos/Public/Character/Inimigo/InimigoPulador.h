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

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

		protected:
			virtual void BeginPlay() override;

			void Jump();

			bool VerificarSeTemChao();

			void SetDirectionRotation(int32 NewDirection);

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_ParedeCollision;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float Velocidade;

			UPROPERTY(EditAnywhere, Category = "Stats")
			float AlturaDoPulo;

			UPROPERTY(EditAnywhere, Category = "Animation")
			TObjectPtr<class UPaperZDAnimationComponent> Comp_Animation;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UAudioComponent> Comp_Audio;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Jump;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Land;

			int32 Direction;

			float DistanciaDoPulo;
			float TempoDoPulo;

			ETargetJump Target;

			FVector StartLocation;
			FVector EndLocation;
			FVector BoxExtent;

			FQuat BoxRotation;

			struct FTimerHandle TH_Jump;
			struct FTimerHandle TH_AnimJump;
			struct FCollisionQueryParams CollisionParams;
			struct FCollisionShape CollisionShape;
			struct FHitResult HitResult;

	#pragma endregion
	
};
