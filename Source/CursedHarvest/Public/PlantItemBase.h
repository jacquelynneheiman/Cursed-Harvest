// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantItemBase.generated.h"

enum EGrowthStatus
{
	Seedling UMETA(DisplayName = "Seedling"),
	Sprout UMETA(DisplayName = "Sprout"),
	Ripening UMETA(DisplayName = "Ripening"),
	Ripe UMETA(DisplayName = "Ripe")
};

enum EPlotStatus
{
	Unused UMETA(DisplayName = "Unused"),
	Plowed UMETA(DisplayName = "Plowed"),
	Planted UMETA(DisplayName = "Planted")
};

UCLASS()
class CURSEDHARVEST_API APlantItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantItemBase();

	/** Overlapping sphere*/
	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* CollisionVolume;

	/** Base Mesh Component*/
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* MyMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
