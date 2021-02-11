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

	/** Seedling to Sprout timer modifier - under 1.0 = faster, over 1.0 = slower 
	Possible uses - better quality tools can dig deeper or prepare lesser quality ground,
	making plant grow faster and vice-versa*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float SeedlingToSproutTimerModifier;

	// Sprout lifetime before transitioning to Ripening
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float RipeningTransitionLength;

	/** Sprout to ripening timer modifier - under 1.0 = faster, over 1.0 = slower
	Possible uses - better quality fertilizer making plant grow faster and vice-versa*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float SproutToRipeningTimerModifier;

	// Sprout lifetime before transitioning to Ripe
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
	float RipeTransitionLength;

	/** Ripening to ripe timer modifier - under 1.0 = faster, over 1.0 = slower
	Possible uses - player pulls weeds which makes plant grow faster and vice-versa*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant Timers")
		float RipeningToRipeTimerModifier;

	

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

	UFUNCTION()
	TEnumAsByte<ECropType> GetCropType();
};
