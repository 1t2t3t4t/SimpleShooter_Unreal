#include "MyPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget(this, HUDClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}
}

void AMyPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	HUDWidget->RemoveFromViewport();
	UUserWidget* Widget = bIsWinner ?  CreateWidget(this, WinGameWidgetClass) : CreateWidget(this, LoseGameWidgetClass);
	if (Widget)
	{
		Widget->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
