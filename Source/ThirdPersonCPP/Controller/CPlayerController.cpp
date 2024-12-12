#include "CPlayerController.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"

ACPlayerController::ACPlayerController()
{
	
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("OptionMenu", IE_Pressed, this, &ACPlayerController::ToggleOptionMenu);

	InputComponent->BindAction("SelectAction", IE_Pressed, this, &ACPlayerController::EnableSelectActionWidget);
	InputComponent->BindAction("SelectAction", IE_Released, this, &ACPlayerController::DisbleSelectActionWidget);
}

void ACPlayerController::ToggleOptionMenu()
{
	CheckNull(OptionMenuClass);

	if (OptionMenu && OptionMenu->IsInViewport())
	{
		OptionMenu->RemoveFromParent();
		OptionMenu = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		return;
	}

	OptionMenu = CreateWidget<UUserWidget>(this, OptionMenuClass);
	CheckNull(OptionMenu);

	OptionMenu->AddToViewport(100);

	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

void ACPlayerController::EnableSelectActionWidget()
{
	//Todo. 
	if (!SelectActionWidget)
	{
		SelectActionWidget = CreateWidget(this, SelectActionWidgetClass);
	}

	if (SelectActionWidget)
	{
		SelectActionWidget->AddToViewport();

		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
	}
}

void ACPlayerController::DiableSelectActionWidget()
{
}
