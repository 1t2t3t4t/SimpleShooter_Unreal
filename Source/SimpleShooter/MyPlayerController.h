#pragma once

#include "MyPlayerController.generated.h"

UCLASS()
class SIMPLESHOOTER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
	float RestartDelay = 3.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> LoseGameWidgetClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinGameWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	FTimerHandle RestartTimer;

	UPROPERTY()
	UUserWidget* HUDWidget;
};
