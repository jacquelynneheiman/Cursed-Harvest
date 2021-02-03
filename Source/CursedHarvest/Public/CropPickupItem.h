// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItemBase.h"
#include "CropPickupItem.generated.h"

UENUM(BlueprintType)
enum ECropType
{
	None UMETA(DisplayName = "None"),
	Cabbage UMETA(DisplayName = "Cabbage"),
	Strawberry UMETA(DisplayName = "Strawberry"),
	Watermelon UMETA(DisplayName = "Watermelon"),
	Corn UMETA(DisplayName = "Corn")
};

/**
 * 
 */
UCLASS()
class CURSEDHARVEST_API ACropPickupItem : public APickupItemBase
{
	GENERATED_BODY()

public:
	ACropPickupItem();

	/** TODO: REMOVE ME AFTER COMBAT SYSTEM IMPLEMENTED - TESTING FOR STATIC WORLD PLACEMENT ONLY */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info")
	TEnumAsByte<ECropType> CropType;
	/** Minimum number of items to spawn when enemy dies*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = '1'))
	int32 MinSpawnNum;
	/** Max number of items to spawn when enemy dies*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = '1'))
	int32 MaxSpawnNum;

	/** Minimum spawn distance units to add to the X value of the dead enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = "-2500", UIMax = "0"))
	int32 MinXSpawnNum;
	/** Minimum spawn distance units to add to the X value of the dead enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = "0", UIMax = "2500"))
	int32 MaxXSpawnNum;

	/** Minimum spawn distance units to add to the Y value of the dead enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = "-2500", UIMax = "0"))
	int32 MinYSpawnNum;
	/** Minimum spawn distance units to add to the Y value of the dead enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crop Info", meta = (UIMin = "0", UIMax = "2500"))
	int32 MaxYSpawnNum;


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	TEnumAsByte<ECropType> GetCropType();		
};
