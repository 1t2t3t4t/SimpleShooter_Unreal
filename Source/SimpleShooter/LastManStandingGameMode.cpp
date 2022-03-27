// Fill out your copyright notice in the Description page of Project Settings.


#include "LastManStandingGameMode.h"

void ALastManStandingGameMode::PawnKilled(APawn* const Pawn)
{
	Super::PawnKilled(Pawn);

	if (APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController()))
	{
		PlayerController->GameHasEnded();
	}
}
