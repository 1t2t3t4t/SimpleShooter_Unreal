// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

namespace BlackboardKey
{
	const FName PlayerLocation = "PlayerLocation";
	const FName StartLocation = "StartLocation";
	const FName LastKnownLocation = "LastKnownLocation";
}

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	RunBehaviorTree(AIBehaviorTree);
	GetBlackboardComponent()->SetValueAsVector(BlackboardKey::StartLocation, GetPawn()->GetActorLocation());
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
