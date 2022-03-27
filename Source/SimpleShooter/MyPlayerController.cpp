#include "MyPlayerController.h"

#include "Blueprint/UserWidget.h"

void AMyPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (UUserWidget* Widget = CreateWidget(this, LoseGameWidgetClass))
	{
		Widget->AddToViewport();
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}
