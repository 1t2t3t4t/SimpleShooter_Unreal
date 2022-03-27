// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::Shoot()
{
	UGameplayStatics::SpawnEmitterAttached(GunMuzzle, MeshComponent, TEXT("MuzzleFlashSocket"));

	FVector Loc;
	FRotator Rot;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;

	OwnerPawn->GetController()->GetPlayerViewPoint(Loc, Rot);

	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	
	const FVector End = Loc + Rot.Vector() * MaxRange;
	const bool bSuccess = GetWorld()->LineTraceSingleByChannel(Result, Loc, End, ECC_GameTraceChannel1, Params);
	
	if (bSuccess)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, GunImpact, Result.ImpactPoint, Result.ImpactNormal.Rotation());
		const FPointDamageEvent DamageEvent(Damage, Result, Rot.Vector(), nullptr);
		Result.GetActor()->TakeDamage(Damage, DamageEvent, OwnerPawn->GetController(), OwnerPawn);
	}
}

