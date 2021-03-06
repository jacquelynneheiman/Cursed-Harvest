// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AC_InteractionBase.h"
#include "CropPickupItem.h"
#include "AC_PlayerInteraction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CURSEDHARVEST_API UAC_PlayerInteraction : public UAC_InteractionBase
{
	GENERATED_BODY()

public:

	UAC_PlayerInteraction();
	/** Reference to the Player Controller*/
	UPROPERTY(BlueprintReadOnly, Category = "Controller")
		class APlayerController* MainPController;

	// Reference to the blueprint - Using blueprint here allows spawning of the BP, so the designer can change parameters as required.
	UPROPERTY(BlueprintReadOnly, Category = "Planting")
	TSubclassOf<class APlantItem> PlantBlueprint;	

	UPROPERTY(BlueprintReadOnly, Category = "Planting")
		APlantItem* Plant;

	UPROPERTY(BlueprintReadOnly, Category = "Planting")
		bool bHasSeed;

	// This is the minimum distance allowed between plants
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planting")
		float PlantMinDistance;

	// This setting allows planting on uneven ground. Higher = greater uneven ground allowed to plant
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planting")
		float GroundVariationAllowance;

	// Reference to the blueprint - Using blueprint here allows spawning of the BP, so the designer can change parameters as required.
	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
		TSubclassOf<class ACropPickupItem> CropPickupBlueprint;

	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
		class ACropPickupItem* PickupItem;	  

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
		bool bPlantIsOverlapped;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
		class APlantItem* OverlappedPlantActor;		

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
		float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
 
		bool bDrawPlantingDebugLine;			
  
	// Particle System for plant and growth stages, seed type 1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX | Crop planting particles")
		class UParticleSystem* PlantParticleSystemS1;
	// Particle System for plant and growth stages, seed type 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX | Crop planting particles")
		UParticleSystem* PlantParticleSystemS2;
	// Particle System for plant and growth stages, seed type 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX | Crop planting particles")
		UParticleSystem* PlantParticleSystemS3;
	// Particle System for plant and growth stages, seed type 4
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX | Crop planting particles")
		UParticleSystem* PlantParticleSystemS4;

	// Particle System for plant and growth stages
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	TArray<UParticleSystem*> PlantParticleSystems;*/

	// Audio component that handles the sound being played
	UPROPERTY()
		class UAudioComponent* AudioComp;

	// Sound for plant and growth stages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		class USoundCue* PlantSuccessSound;

	// Sound for plant and growth stages
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		class USoundCue* PlantFailSound;	
 

	UPROPERTY()
	AMainCharacter* mainCharacter;

	// Line trace forward from player for interaction	
	UFUNCTION()
	void CheckForValidPlantSpot(AMainCharacter* mainChar);

	// Get z location of ground at x,y
	UFUNCTION(BlueprintPure, meta = (AdvancedDisplay = "bDrawDebugLines"))
		float GetSurface(FVector2D Point, bool bDrawDebugLines = false);

	float GroundLoc;

public:
	UFUNCTION()
	void DisplayCropTypeUponPickup(TEnumAsByte<ECropType> pickup);

	UFUNCTION()
	void SetPlantOverlapStatus(class APlantItem* plantActor, bool overlapStatus);

	UFUNCTION()
	void CheckCanPlant(FVector_NetQuantize loc, FRotator rot);

	UFUNCTION()
	void TryToPlant(FVector_NetQuantize loc, FRotator rot);

	UFUNCTION()
	void SpawnPickupItems(AMainCharacter* mainChar, ECropType cropType);

	UFUNCTION()
		UParticleSystem* GetParticleSystemToSpawn(ECropType CropTypeToSpawn);

	UFUNCTION()
	void SpawnParticleSystem(UParticleSystem* ParticleSystem, FVector Location, FRotator Rotation);

	
};
