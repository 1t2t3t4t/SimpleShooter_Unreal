﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int CurrentAmmo = 10;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool ShootLineTrace(const FVector& Loc, FRotator Rot, FHitResult& Result) const;

	void Shoot();
	void PlayShootSound() const;
	void PlayImpactSound(const FVector& Location) const;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* GunMuzzle;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* GunImpact;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 100.f;

	UPROPERTY(EditAnywhere)
	float Damage = 10.f;
};
