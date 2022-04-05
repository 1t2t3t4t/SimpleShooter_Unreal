// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

bool AGun::ShootLineTrace(const FVector& Loc, const FRotator Rot, FHitResult& Result) const
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	FVector ShootDir = UKismetMathLibrary::InverseTransformDirection(GetActorTransform(), Rot.Vector() * MaxRange);
	ShootDir.X += FMath::RandRange(MinRand, MaxRand);
	ShootDir.Z += FMath::RandRange(MinRand, MaxRand);
	
	const FVector End = Loc + UKismetMathLibrary::TransformDirection(GetActorTransform(), ShootDir);
	return GetWorld()->LineTraceSingleByChannel(Result, Loc, End, ECC_GameTraceChannel1, Params);
}

void AGun::Shoot()
{
	if (CurrentAmmo == 0) return;
	UGameplayStatics::SpawnEmitterAttached(GunMuzzle, MeshComponent, TEXT("MuzzleFlashSocket"));

	FVector Loc;
	FRotator Rot;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return;
	OwnerPawn->GetController()->GetPlayerViewPoint(Loc, Rot);

	PlayShootSound();
	CurrentAmmo--;
	
	FHitResult Result;
	if (ShootLineTrace(Loc, Rot, Result) && OwnerPawn->GetController())
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, GunImpact, Result.ImpactPoint, Result.ImpactNormal.Rotation());
		const FPointDamageEvent DamageEvent(Damage, Result, Rot.Vector(), nullptr);
		Result.GetActor()->TakeDamage(Damage, DamageEvent, OwnerPawn->GetController(), OwnerPawn);
		PlayImpactSound(Result.ImpactPoint);
	}
}

void AGun::PlayShootSound() const
{
	UGameplayStatics::SpawnSoundAttached(ShootSound, MeshComponent, TEXT("MuzzleFlashSocket"));
}

void AGun::PlayImpactSound(const FVector& Location) const
{
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, Location);
}

void AGun::Reload()
{
	const int VacantAmmo = FMath::Min(GetCapAmmo() - CurrentAmmo, MagazineAmmo);
	MagazineAmmo -= VacantAmmo;
	CurrentAmmo += VacantAmmo;
}

int AGun::GetCapAmmo() const
{
	return CapAmmo;
}
