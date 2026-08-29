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

		protected:
			virtual void BeginPlay() override;

			UFUNCTION()
			void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

			void OpenDoor();

			virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

			void PlaySound(USoundBase* Sound);

	#pragma endregion

	#pragma region Variables

		public:	

		protected:
			UPROPERTY(EditAnywhere)
			TObjectPtr<class UPaperSpriteComponent> Comp_Sprite;

			UPROPERTY(EditAnywhere)
			TObjectPtr<class UBoxComponent> Comp_BoxCollision;

			UPROPERTY(EditAnywhere, Category = "Mapa")
			TSoftObjectPtr<UWorld> NextLevel;

			UPROPERTY(EditAnywhere, Category = "Sprites")
			TObjectPtr<class UPaperSprite> Sprite_Open;

			UPROPERTY(EditAnywhere, Category = "Sprites")
			TObjectPtr<class UPaperSprite>  Sprite_Closed;

			UPROPERTY(EditAnywhere)
			TObjectPtr<UAudioComponent> Comp_Audio;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_BaseFechado;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_BaseAberto;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Abriu;

			UPROPERTY(EditAnywhere, Category = "Sound")
			TObjectPtr<USoundBase> SB_Entrar;

			UPROPERTY()
			TObjectPtr<class AMainGameMode> GameMode;

			UPROPERTY()
			TObjectPtr<class UGame_Save> GameData;

			bool bIsOpen;

			TObjectPtr<class IInterface_SaveGame> SaveGameInterface;

			TObjectPtr<class UMain_Game_Instance> GameInstance;

	#pragma endregion
	
};
