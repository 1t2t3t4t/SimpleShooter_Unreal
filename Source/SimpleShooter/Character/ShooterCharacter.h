// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gun.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "SimpleShooter/Item/Item.h"
#include "ShooterCharacter.generated.h"

class UInventoryComponent;

UCLASS(Abstract)
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void EquipCurrentGun();

	UFUNCTION()
	void OnBeginCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                      bool bFromSweep, const FHitResult& SweepResult);
	void PickupItem(const AItem* Item) const;
	void PickupGun(AGun* ToPickUpGun) const;
	void ChangeWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void PressedShoot();
	void ReleasedShoot();
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;
	void Dead();
	void Reload();

	UFUNCTION(BlueprintPure)
	bool GetAlive() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
	int GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	
	void Shoot() const;

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> StartingGunClass;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Animations")
	UAnimMontage* ReloadMontage;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	bool bRevert = true;
	
	UPROPERTY()
	AGun* Gun;

	bool IsReloading = false;

	bool Shooting = false;

	void SetupCollision();
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
};
