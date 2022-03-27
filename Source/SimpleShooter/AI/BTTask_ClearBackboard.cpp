// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBackboard.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_ClearBackboard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const FName SelectedKey = GetSelectedBlackboardKey();
	OwnerComp.GetBlackboardComponent()->ClearValue(SelectedKey);
	return EBTNodeResult::Succeeded;
}
