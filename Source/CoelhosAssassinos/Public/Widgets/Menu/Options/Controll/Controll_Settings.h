#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Controll_Settings.generated.h"

class UInputKeySelector;
class UInputMappingContext;
class UInputAction;
struct FKey;
struct FInputChord;

UCLASS()	
class COELHOSASSASSINOS_API UControll_Settings : public UUserWidget
{
	GENERATED_BODY()

	#pragma region Functions

		public:
			void Init();

		protected:
			UFUNCTION()
			void OnKeySelected_MoveLeft(FInputChord SelectedKey);

			UFUNCTION()
			void OnKeySelected_MoveLeft_Secondary(FInputChord SelectedKey);

			UFUNCTION()
			void OnKeySelected_MoveRight(FInputChord SelectedKey);

			UFUNCTION()
			void OnKeySelected_MoveRight_Secondary(FInputChord SelectedKey);

			UFUNCTION()
			void OnKeySelected_Jump(FInputChord SelectedKey);

			UFUNCTION()
			void OnKeySelected_Jump_Secondary(FInputChord SelectedKey);

			void ProcessRemap(FName MappingName, FKey NewKey);

			void SetCurrentBinds(FName ActionName, TObjectPtr<UInputKeySelector> KeySelector);

			void AddMappingContext(bool bAddOrRemove);

			virtual void NativeDestruct() override;

	#pragma endregion

	#pragma region Variaveis

		public:

		protected:
			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_MoveLeft;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_MoveLeft_Secondary;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_MoveRight;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_MoveRight_Secondary;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_Jump;

			UPROPERTY(meta = (BindWidget))
			TObjectPtr<UInputKeySelector> KS_Jump_Secondary;

			UPROPERTY(EditAnywhere, Category = "Mapping Context")
			TObjectPtr<UInputMappingContext> IMC;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName MoveLeft;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName MoveLeft_Secundary;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName MoveRight;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName MoveRight_Secundary;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName Jump;

			UPROPERTY(EditAnywhere, Category = "Input")
			FName Jump_Secundary;

	#pragma endregion
	
};
