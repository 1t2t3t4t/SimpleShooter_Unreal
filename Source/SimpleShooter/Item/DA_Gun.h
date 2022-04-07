// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DA_Gun.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLESHOOTER_API UDA_Gun : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGun> GunClass;
};
