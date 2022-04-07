// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickable.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SIMPLESHOOTER_API AItem : public AActor, public IPickable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataAsset* ItemData;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Pick_Implementation(const AActor* ByActor) override;

	UFUNCTION(BlueprintPure)
	FString GetItemName() const;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	FString Name;
};
