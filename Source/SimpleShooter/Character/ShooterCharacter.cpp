// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "SimpleShooter/Item/DA_Ammo.h"
#include "SimpleShooter/Item/DA_Gun.h"
#include "SimpleShooter/Item/InventoryComponent.h"
#include "SimpleShooter/Item/Item.h"
#include "SimpleShooter/Item/Pickable.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	SpringArmComponent->SetupAttachment(RootComponent);
	Health = MaxHealth;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

	InventoryComponent->AppendGunClass(StartingGunClass);
	SetupCollision();
	EquipCurrentGun();
}

void AShooterCharacter::EquipCurrentGun()
{
	if (Gun)
	{
		Gun->SetActorHiddenInGame(true);
	}
	Gun = InventoryComponent->GetCurrentGun();
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetActorHiddenInGame(false);
}

void AShooterCharacter::SetupCollision()
{
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AShooterCharacter::OnBeginCollision);
}

void AShooterCharacter::OnBeginCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin overlap from callback"));

	if (OtherActor->Implements<UPickable>() && IsPlayerControlled())
	{
		IPickable::Execute_Pick(OtherActor, this);
		if (const AItem* Item = Cast<AItem>(OtherActor))
		{
			PickupItem(Item);
		}
	}
}

void AShooterCharacter::PickupItem(const AItem* Item) const
{
	if (const UDA_Ammo* AmmoInfo = Cast<UDA_Ammo>(Item->ItemData))
	{
		Gun->MagazineAmmo += AmmoInfo->Amount;
	}
	else if (const UDA_Gun* GunItem = Cast<UDA_Gun>(Item->ItemData))
	{
		InventoryComponent->AppendGunClass(GunItem->GunClass);
	}
}

void AShooterCharacter::PickupGun(AGun* ToPickUpGun) const
{
	InventoryComponent->AppendGun(ToPickUpGun);
}

void AShooterCharacter::ChangeWeapon()
{
	InventoryComponent->ChangeWeapon(true);
	EquipCurrentGun();
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Shooting)
	{
		Shoot();
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::PressedShoot);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AShooterCharacter::ReleasedShoot);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShooterCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("ChangeWeaponUp"), IE_Pressed, this, &AShooterCharacter::ChangeWeapon);
}

void AShooterCharacter::PressedShoot()
{
	Shooting = true;
}

void AShooterCharacter::ReleasedShoot()
{
	Shooting = false;
}

float AShooterCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (!GetAlive()) return 0.f;
	
	const float DamageApplied = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	const float MinDamage = FMath::Min(Health, DamageApplied);
	Health -= MinDamage;

	if (!GetAlive())
	{
		Dead();
	}
	return MinDamage;
}

void AShooterCharacter::Dead()
{
	ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
	GameMode->PawnKilled(this);
	
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShooterCharacter::Reload()
{
	if (Gun->MagazineAmmo == 0)
	{
		return;
	}
	IsReloading = true;
	PlayAnimMontage(ReloadMontage);
	Gun->Reload();
}

bool AShooterCharacter::GetAlive() const
{
	return Health > 0.f;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

int AShooterCharacter::GetCurrentAmmo() const
{
	return Gun->CurrentAmmo;
}

void AShooterCharacter::FinishReloading()
{
	IsReloading = false;
}

void AShooterCharacter::Shoot() const
{
	if (IsReloading)
	{
		return;
	}
	Gun->Shoot();
}

void AShooterCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AShooterCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AShooterCharacter::LookUp(float Value)
{
	const int Revert = bRevert ? -1 : 1;
	AddControllerPitchInput(Value * Revert);
}


