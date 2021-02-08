// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlantItemBase.h"
#include "CropPickupItem.h"
#include "PlantItem.generated.h"

/**
 * 
 */
UCLASS()
class CURSEDHARVEST_API APlantItem : public APlantItemBase
{
	GENERATED_BODY()

public:
	APlantItem();

	// Define enums
	EGrowthStatus GrowthStatus;
	EPlotStatus PlotStatus;
	ECropType CropType;

	UPROPERTY(BlueprintReadOnly)
		class AMainCharacter* MainCharRef;
	UPROPERTY(BlueprintReadOnly)
		class UAC_PlayerInteraction* InteractionComp;

	UStaticMesh* MeshToUse;	
	
	FTimerHandle PlowTimer;	
	FTimerHandle SeedlingToSproutTimer;
	FTimerHandle SproutToRipeningTimer;
	FTimerHandle RipeningToRipeTimer;

	
	// Length of the plow anim - TODO: Remove me and use an anim notify to indicate the end of plowing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float PlowTimerLength;

	// Seedling lifetime before transitioning to a Sprout
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float SproutTransitionLength;

	// Sprout lifetime before transitioning to Ripening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float RipeningTransitionLength;

	// Sprout lifetime before transitioning to Ripe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float RipeTransitionLength;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void InteractWithPlant();

	UFUNCTION()
	void EndPlowing();

	UFUNCTION()
	void TransitionToSprout();

	UFUNCTION()
	void TransitionToRipening();

	UFUNCTION()
	void TransitionToRipe();

	 
	UFUNCTION()
	void SetMesh(FString MeshPath);
};
