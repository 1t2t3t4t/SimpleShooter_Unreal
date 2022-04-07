// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "SimpleShooter/Character/Gun.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AGun* UInventoryComponent::GetCurrentGun() const
{
	if (CurrentGunIndex > Guns.Num())
	{
		return nullptr;
	}
	return Guns[CurrentGunIndex];
}

void UInventoryComponent::ChangeWeapon(const bool Next)
{
	const int Addition = Next ? 1 : -1;
	CurrentGunIndex = (CurrentGunIndex + Addition) % Guns.Num();
}

void UInventoryComponent::AppendGun(AGun* Gun)
{
	Guns.Add(Gun);
}

AGun* UInventoryComponent::AppendGunClass(TSubclassOf<AGun> GunClass)
{
	AGun* Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->SetOwner(GetOwner());
	AppendGun(Gun);
	return Gun;
}

AGun* UInventoryComponent::RemoveCurrentGun()
{
	const int NewIndex = (CurrentGunIndex + 1) % Guns.Num();
	Guns.RemoveAt(CurrentGunIndex);
	CurrentGunIndex = NewIndex;
	return GetCurrentGun();
}


