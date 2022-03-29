// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleShooterGameModeBase.h"
#include "LastManStandingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API ALastManStandingGameMode : public ASimpleShooterGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* const Pawn) override;
	void EndGame(const bool bIsPlayerWin) const;

protected:
	virtual void BeginPlay() override;
	int32 CalculateEnemies() const;
};
