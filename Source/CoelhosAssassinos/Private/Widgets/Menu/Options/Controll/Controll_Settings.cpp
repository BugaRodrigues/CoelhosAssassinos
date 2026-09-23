#include "Widgets/Menu/Options/Controll/Controll_Settings.h"
#include "System/RemappingSystem/InputRemappingSystem.h"
#include "Components/InputKeySelector.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"

void UControll_Settings::Init()
{
	if (MoveLeft.IsNone() || MoveRight.IsNone() || Jump.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Nomes Invalidos!!!"));
		return;
	}

	SetIsEnabled(true);

	AddMappingContext(true);

	SetCurrentBinds(MoveLeft, KS_MoveLeft);
	SetCurrentBinds(MoveLeft_Secundary, KS_MoveLeft_Secondary);
	SetCurrentBinds(MoveRight, KS_MoveRight);
	SetCurrentBinds(MoveRight_Secundary, KS_MoveRight_Secondary);
	SetCurrentBinds(Jump, KS_Jump);
	SetCurrentBinds(Jump_Secundary, KS_Jump_Secondary);

	KS_MoveLeft->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_MoveLeft);
	KS_MoveLeft_Secondary->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_MoveLeft);
	KS_MoveRight->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_MoveRight);
	KS_MoveRight_Secondary->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_MoveRight);
	KS_Jump->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_Jump);
	KS_Jump_Secondary->OnKeySelected.AddDynamic(this, &UControll_Settings::OnKeySelected_Jump);
}

void UControll_Settings::OnKeySelected_MoveLeft(FInputChord SelectedKey) { ProcessRemap(MoveLeft, SelectedKey.Key); }

void UControll_Settings::OnKeySelected_MoveLeft_Secondary(FInputChord SelectedKey) { ProcessRemap(MoveLeft, SelectedKey.Key); }

void UControll_Settings::OnKeySelected_MoveRight(FInputChord SelectedKey) { ProcessRemap(MoveRight, SelectedKey.Key); }

void UControll_Settings::OnKeySelected_MoveRight_Secondary(FInputChord SelectedKey) { ProcessRemap(MoveRight, SelectedKey.Key); }

void UControll_Settings::OnKeySelected_Jump(FInputChord SelectedKey) { ProcessRemap(Jump, SelectedKey.Key); }

void UControll_Settings::OnKeySelected_Jump_Secondary(FInputChord SelectedKey) { ProcessRemap(Jump, SelectedKey.Key); }

void UControll_Settings::ProcessRemap(FName MappingName, FKey NewKey)
{
	ULocalPlayer* LP = GetOwningLocalPlayer();
	if (!LP) return;

	UInputRemappingSystem* RSS = LP->GetSubsystem<UInputRemappingSystem>();
	if (!RSS) return;

	RSS->RemapKey(MappingName, NewKey);
	RSS->SaveSettings();
}

void UControll_Settings::SetCurrentBinds(FName ActionName, TObjectPtr<UInputKeySelector> KeySelector)
{
	ULocalPlayer* LP = GetOwningLocalPlayer();
	if (!LP) return;

	UInputRemappingSystem* RSS = LP->GetSubsystem<UInputRemappingSystem>();
	if (!RSS) return;

	KeySelector->SetSelectedKey(RSS->GetCurrentKey(ActionName)); 
}

void UControll_Settings::AddMappingContext(bool bAddOrRemove)
{
	if (!IMC) return;

	APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!PC) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (!Subsystem || Subsystem->HasMappingContext(IMC)) return;

	FModifyContextOptions Options;
	Options.bNotifyUserSettings = true;

	bAddOrRemove ? Subsystem->AddMappingContext(IMC, 0, Options) : Subsystem->RemoveMappingContext(IMC);
}

void UControll_Settings::NativeDestruct()
{
	Super::NativeDestruct();

	AddMappingContext(false);

	KS_MoveLeft->OnKeySelected.RemoveAll(this);
	KS_MoveRight->OnKeySelected.RemoveAll(this);
	KS_Jump->OnKeySelected.RemoveAll(this);
}
