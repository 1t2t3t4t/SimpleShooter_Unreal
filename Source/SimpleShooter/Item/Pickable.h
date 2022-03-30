// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickable.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UPickable : public UInterface
{
	GENERATED_BODY()
};

class SIMPLESHOOTER_API IPickable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category= "Pick up")
	void Pick(const AActor* ByActor);
};
