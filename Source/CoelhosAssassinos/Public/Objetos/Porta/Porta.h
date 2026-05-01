#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Porta.generated.h"

UCLASS()
class COELHOSASSASSINOS_API APorta : public AActor
{
	GENERATED_BODY()
	
	#pragma region Functions

		public:	
			APorta();

			//virtual void Tick(float DeltaTime) override;

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			void OpenDoor();

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			class UPaperSpriteComponent* Comp_Sprite;

			UPROPERTY(EditAnywhere)
			class UBoxComponent* Comp_BoxCollision;

			UPROPERTY(EditAnywhere, Category = "Mapa")
			TSoftObjectPtr<UWorld> NextLevel;

			UPROPERTY(EditAnywhere, Category = "Sprites")
			class UPaperSprite* Sprite_Open;

			UPROPERTY(EditAnywhere, Category = "Sprites")
			class UPaperSprite* Sprite_Closed;

			bool bIsOpen;

			UPROPERTY()
			class AMainGameMode* GameMode;

			UPROPERTY()
			class UGame_Save* SaveData;

			UPROPERTY()
			TScriptInterface<class IInterface_SaveGame> Interface_SG;

			class IInterface_SaveGame* SaveGameInterface;

	#pragma endregion
	
};
