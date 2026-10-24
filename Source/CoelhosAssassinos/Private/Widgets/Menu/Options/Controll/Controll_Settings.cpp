#include "Widgets/Menu/Options/Controll/Controll_Settings.h"
#include "Character/Controller/Player/Player_Controller.h"
#include "System/RemappingSystem/InputRemappingSystem.h"
#include "Components/InputKeySelector.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"

void UControll_Settings::Init()
{
#if PLATFORM_DESKTOP
	if (MoveLeft.IsNone() || MoveRight.IsNone() || Jump.IsNone())
	{
		UE_LOG(LogTemp, Error, TEXT("Nomes Invalidos!!!"));
		return;
	}

	SetIsEnabled(true);

	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->AddOrRemoveImc(true);
	}

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
#endif // PLATFORM_DESKTOP
}

void UControll_Settings::OnKeySelected_MoveLeft(FInputChord SelectedKey) 
{ 
#if PLATFORM_DESKTOP
	ProcessRemap(MoveLeft, SelectedKey.Key);
#endif // PLATFORM_DESKTOP
}

void UControll_Settings::OnKeySelected_MoveLeft_Secondary(FInputChord SelectedKey) 
{ 
#if PLATFORM_DESKTOP
	ProcessRemap(MoveLeft, SelectedKey.Key); 
#endif
}

void UControll_Settings::OnKeySelected_MoveRight(FInputChord SelectedKey) 
{ 
#if PLATFORM_DESKTOP
	ProcessRemap(MoveRight, SelectedKey.Key); 
#endif
}

void UControll_Settings::OnKeySelected_MoveRight_Secondary(FInputChord SelectedKey) 
{ 
#if PLATFORM_DESKTOP
	ProcessRemap(MoveRight, SelectedKey.Key); 
#endif
}

void UControll_Settings::OnKeySelected_Jump(FInputChord SelectedKey) 
{
#if PLATFORM_DESKTOP
	ProcessRemap(Jump, SelectedKey.Key); 
#endif
}

void UControll_Settings::OnKeySelected_Jump_Secondary(FInputChord SelectedKey) 
{ 
#if PLATFORM_DESKTOP
	ProcessRemap(Jump, SelectedKey.Key); 
#endif
}

void UControll_Settings::ProcessRemap(FName MappingName, FKey NewKey)
{
#if PLATFORM_DESKTOP
	ULocalPlayer* LP = GetOwningLocalPlayer();
	if (!LP) return;

	UInputRemappingSystem* RSS = LP->GetSubsystem<UInputRemappingSystem>();
	if (!RSS) return;

	RSS->RemapKey(MappingName, NewKey);
	RSS->SaveSettings();
#endif
}

void UControll_Settings::SetCurrentBinds(FName ActionName, TObjectPtr<UInputKeySelector> KeySelector)
{
#if PLATFORM_DESKTOP
	ULocalPlayer* LP = GetOwningLocalPlayer();
	if (!LP) return;

	UInputRemappingSystem* RSS = LP->GetSubsystem<UInputRemappingSystem>();
	if (!RSS) return;

	KeySelector->SetSelectedKey(RSS->GetCurrentKey(ActionName)); 
#endif
}

void UControll_Settings::NativeDestruct()
{
	Super::NativeDestruct();
#if PLATFORM_DESKTOP
	if (APlayer_Controller* Pc = Cast<APlayer_Controller>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		Pc->AddOrRemoveImc(false);
	}

	KS_MoveLeft->OnKeySelected.RemoveAll(this);
	KS_MoveRight->OnKeySelected.RemoveAll(this);
	KS_Jump->OnKeySelected.RemoveAll(this);
#endif
}
