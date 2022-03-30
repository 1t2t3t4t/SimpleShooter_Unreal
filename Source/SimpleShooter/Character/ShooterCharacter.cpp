// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Components/CapsuleComponent.h"
#include "SimpleShooter/SimpleShooterGameModeBase.h"
#include "SimpleShooter/Item/DA_Ammo.h"
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
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->SetOwner(this);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	SetupCollision();
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
		PickupItem(Cast<AItem>(OtherActor));
	}
}

void AShooterCharacter::PickupItem(const AItem* Item)
{
	if (Item == nullptr) return;

	if (const UDA_Ammo* AmmoInfo = Cast<UDA_Ammo>(Item->ItemData))
	{
		Gun->CurrentAmmo += AmmoInfo->Amount;
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AShooterCharacter::Shoot);
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

void AShooterCharacter::Shoot()
{
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


