// Fill out your copyright notice in the Description page of Project Settings.


#include "CropPickupItem.h"
#include "MainCharacter.h"
#include "AC_PlayerInteraction.h"

ACropPickupItem::ACropPickupItem()
{
	CropType = ECropType::None;

	MinSpawnNum = 1;
	MaxSpawnNum = 5;

	MinXSpawnNum = -300;
	MaxXSpawnNum = 300;

	MinYSpawnNum = -300;
	MaxYSpawnNum = 300;

}

void ACropPickupItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT(" ACropPickupItem::OnOverlapBegin")); 

	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	if (MainCharacter)
	{
		/** PICK UP PLANT, ADD TO INVENTORY AND DESTROY THE PICKUP ACTOR*/
		   // Get CropType
		TEnumAsByte<ECropType> ItemCropType;
		ItemCropType = GetCropType();

		// Convert to string for log
		FString ItemStr;
		ItemStr = UEnum::GetDisplayValueAsText(ItemCropType).ToString();
		UE_LOG(LogTemp, Warning, TEXT("CropPickupItem: You picked up a %s."), *ItemStr);

		// Pass croptype to MainCharacter and add to inventory
		MainCharacter->InteractionComp->DisplayCropTypeUponPickup(ItemCropType); // TODO: Remove me before prod. Was just for proof of concept
		//MainCharacter->InventoryComp->AddToInventory(itemCropType, Qty); 
		MainCharacter->AddItemToInventory();

		// Call parent function OnBeginOverlap to destroy the pickup item
		Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	}
}

TEnumAsByte<ECropType> ACropPickupItem::GetCropType()
{
	return CropType;
}

