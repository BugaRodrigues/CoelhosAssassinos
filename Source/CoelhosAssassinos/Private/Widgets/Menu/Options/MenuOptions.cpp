#include "Widgets/Menu/Options/MenuOptions.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMenuOptions::NativeConstruct()
{
	Super::NativeConstruct();

	BTN_Video->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Video);
	BTN_Audio->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Audio);
	BTN_Controls->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Controls);
	BTN_Voltar->OnClicked.AddDynamic(this, &UMenuOptions::OnClicked_BTN_Voltar);

	WS_MenuOptions->SetActiveWidgetIndex(0);
}

void UMenuOptions::NativeDestruct()
{
	Super::NativeDestruct();

	BTN_Video->OnClicked.RemoveDynamic(this, &UMenuOptions::OnClicked_BTN_Video);
	BTN_Audio->OnClicked.RemoveDynamic(this, &UMenuOptions::OnClicked_BTN_Audio);
	BTN_Controls->OnClicked.RemoveDynamic(this, &UMenuOptions::OnClicked_BTN_Controls);
	BTN_Voltar->OnClicked.RemoveDynamic(this, &UMenuOptions::OnClicked_BTN_Voltar);
}

void UMenuOptions::OnClicked_BTN_Video()
{
	PlaySound(SB_Click);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Video"));

	WS_MenuOptions->SetActiveWidgetIndex(0);
}

void UMenuOptions::OnClicked_BTN_Audio()
{
	PlaySound(SB_Click);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Audio"));

	WS_MenuOptions->SetActiveWidgetIndex(1);
}

void UMenuOptions::OnClicked_BTN_Controls()
{
	PlaySound(SB_Click);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Controls"));

	WS_MenuOptions->SetActiveWidgetIndex(2);
}

void UMenuOptions::OnClicked_BTN_Voltar()
{
	PlaySound(SB_Click);

	if (UWidgetSwitcher* WS_MenuPrincipal = Cast<UWidgetSwitcher>(GetParent()))
	{
		WS_MenuPrincipal->SetActiveWidgetIndex(0);
	}
}

void UMenuOptions::PlaySound(USoundBase* Sound)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(GetWorld(), Sound);
}
