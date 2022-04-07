// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AGun;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMPLESHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintPure)
	AGun* GetCurrentGun() const;

	UFUNCTION(BlueprintCallable)
	void AppendGun(AGun* Gun);

	UFUNCTION(BlueprintCallable)
	AGun* AppendGunClass(TSubclassOf<AGun> GunClass);

	UFUNCTION(BlueprintCallable)
	AGun* RemoveCurrentGun();
	
private:
	UPROPERTY(VisibleAnywhere)
	TArray<AGun*> Guns;

	int CurrentGunIndex = 0;
};
