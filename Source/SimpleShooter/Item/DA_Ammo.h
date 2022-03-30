// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_Ammo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLESHOOTER_API UDA_Ammo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Amount;
};
