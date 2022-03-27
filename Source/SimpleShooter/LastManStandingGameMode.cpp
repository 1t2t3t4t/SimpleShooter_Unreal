// Fill out your copyright notice in the Description page of Project Settings.


#include "LastManStandingGameMode.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "Character/ShooterCharacter.h"

void ALastManStandingGameMode::BeginPlay()
{
	Super::BeginPlay();
	EnemyCount = CalculateEnemies();
}

int32 ALastManStandingGameMode::CalculateEnemies() const
{
	int32 Result = 0;
	for (const auto Controller : TActorRange<AAIController>(GetWorld()))
	{
		if (const AShooterCharacter* ShooterCharacter = Cast<AShooterCharacter>(Controller->GetPawn()))
		{
			if (ShooterCharacter->GetAlive())
			{
				Result++;
			}
		}
	}
	return Result;
}

void ALastManStandingGameMode::PawnKilled(APawn* const Pawn)
{
	Super::PawnKilled(Pawn);

	if (Cast<APlayerController>(Pawn->GetController()))
	{
		EndGame(false);
	}
	else
	{
		EnemyCount--;
	}

	if (EnemyCount == 0)
	{
		EndGame(true);
	}
	
}

void ALastManStandingGameMode::EndGame(const bool bIsPlayerWin) const
{
	for (const auto Controller : TActorRange<AController>(GetWorld()))
	{
		Controller->GameHasEnded(Controller->GetPawn(), Controller->IsPlayerController() == bIsPlayerWin);
	}
}

