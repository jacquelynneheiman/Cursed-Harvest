// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_PlayerInteraction.h"
#include "PlantItem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

ECropType CropType;

UAC_PlayerInteraction::UAC_PlayerInteraction()
{

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("Class'/Game/Blueprints/Plant/PlantItem_BP.PlantItem_BP_C'"));
	if (bpClassFinder.Object)
	{
		PlantBlueprint = bpClassFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinderPickup(TEXT("Class'/Game/Blueprints/Pickup/CropPickupItem_BP.CropPickupItem_BP_C'"));	 
	if (bpClassFinderPickup.Object)
	{
		CropPickupBlueprint = bpClassFinderPickup.Object;
	}
	
	bPlantIsOverlapped = false;
	bHasSeed = false;
	PlantMinDistance = 300.f;
	GroundVariationAllowance = 10.f;
	TraceDistance = 2000.f;	 
}

void UAC_PlayerInteraction::DisplayCropTypeUponPickup(TEnumAsByte<ECropType> pickup)
{
	/** This is just a test to show that the pickup can "talk" to the player and tell them what was picked up.
	This can be used for scoring, etc. */
	FString ItemStr;
	ItemStr = UEnum::GetDisplayValueAsText(pickup).ToString();
	UE_LOG(LogTemp, Warning, TEXT("AC_PlayerInteractionSystemItem - Crop type picked up: %s"), *ItemStr);
}

// Called from PlantItem
void UAC_PlayerInteraction::SetPlantOverlapStatus(APlantItem* plantActor, bool overlapStatus)
{
	if (plantActor)
	{
		OverlappedPlantActor = plantActor;
		bPlantIsOverlapped = overlapStatus;
		//UE_LOG(LogTemp, Warning, TEXT("OVERLAPPED"));
	}
	else
	{
		OverlappedPlantActor = plantActor;
		bPlantIsOverlapped = overlapStatus;
		//UE_LOG(LogTemp, Warning, TEXT("NOT OVERLAPPED"));
	}
}

void UAC_PlayerInteraction::CheckForValidPlantSpot(AMainCharacter* mainChar)
{
	
	FVector Loc;
	FRotator Rot;
	//FHitResult Hit;
	// Get player location and rotation
	mainCharacter = Cast<AMainCharacter>(mainChar);
	mainCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);
	

	bool bPlantLoc;
	FHitResult HitResult;
	MainPController = UGameplayStatics::GetPlayerController(this, 0);
	bPlantLoc = MainPController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), 0, HitResult);

	if (bPlantLoc)
	{
		/**Get ground Z location*/
		// Define the X, Y var
		FVector2D GroundXY;
		// Get the X, Y of the line trace
		GroundXY = FVector2D(HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y);
		// Get the ground (Z) location under the line trace impact point
		GroundLoc = GetSurface(GroundXY, false); 
		// Is hit point within x units (groundVariationAllowance) of the ground (Z) location? This allows some leeway in planting on non-flat ground
		if (GroundLoc + GroundVariationAllowance > HitResult.ImpactPoint.Z && GroundLoc - GroundVariationAllowance < HitResult.ImpactPoint.Z)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can plant on this surface."));
			FVector_NetQuantize PlantLoc;
			//PlantLoc = Hit.ImpactPoint;

			// TEST
			
				UE_LOG(LogTemp, Warning, TEXT("Hit location = %s"), *HitResult.ImpactPoint.ToString());
				PlantLoc = HitResult.ImpactPoint;
				CheckCanPlant(PlantLoc, Rot);
				// TODO: Remove below for prod
				DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(25, 25, 25), FColor::Emerald, false, 3.f);
			 
			// END TEST

			// See if player meets requirements to plant a seed
			//CheckCanPlant(PlantLoc, Rot); 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CAN'T PLANT HERE"));
		}
		//UE_LOG(LogTemp, Warning, TEXT("Ground location Z(height): %f"), GroundLoc);
	//}
	}
}

// Gets the ground location at X,Y - Used for Determining the location to plant a seed
float UAC_PlayerInteraction::GetSurface(FVector2D Point, bool bDrawDebugLines)
{
	UWorld* World{ this->GetWorld() };

	if (World)
	{
		FVector StartLocation{ Point.X, Point.Y, 1000 };    // Raytrace starting point.
		FVector EndLocation{ Point.X, Point.Y, 0 };            // Raytrace end point.

		// Raytrace for overlapping actors.
		FHitResult HitResult;
		World->LineTraceSingleByObjectType(
			OUT HitResult,
			StartLocation,
			EndLocation,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
			FCollisionQueryParams()
		);

		// Draw debug line.
		if (bDrawDebugLines)
		{
			FColor LineColor;

			if (HitResult.GetActor()) LineColor = FColor::Red;
			else LineColor = FColor::Green;

			DrawDebugLine(
				World,
				StartLocation,
				EndLocation,
				LineColor,
				true,
				5.f,
				0.f,
				10.f
			);
		}

		// Return Z location.
		if (HitResult.GetActor()) return HitResult.ImpactPoint.Z;
	}
	return 0;
}

// Check if player meets the requirements to plant (has a seed in inventory)
void UAC_PlayerInteraction::CheckCanPlant(FVector_NetQuantize loc, FRotator rot)
{	
	// TODO: REMOVE ME - These will be set from the Menu class according to inventory and seed type
	bHasSeed = true;
	CropType = ECropType::Cabbage;

	// Try to plant if player has seed
	if (bHasSeed == true)
	{
		// Find all plants in world
		TArray<AActor*> FoundPlants;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlantItem::StaticClass(), FoundPlants);

		// Was more than 1 plant found?
		if (FoundPlants.Num() > 1)
		{
			for (AActor* FoundPlant : FoundPlants)
			{
				FVector PlantLoc;
				// Get location of the found plant
				PlantLoc = FoundPlant->GetActorLocation();
				float dist;
				dist = (PlantLoc - loc).Size();
				// Is the found plant too close to where we want to plant a new seed?
				if (dist > PlantMinDistance)
				{
					TryToPlant(loc, rot);					
				}
				else
				{
					// TODO: Need proper on-screen UI message here.
					UE_LOG(LogTemp, Warning, TEXT("Too close to another plant! Please try again further away."));
				}
			}
		}
		else
		{
			TryToPlant(loc, rot);
		}		
	}
	else
	{
		// Notify user they have no seeds
		UE_LOG(LogTemp, Warning, TEXT("You don't have any seeds"));
	}
}

void UAC_PlayerInteraction::TryToPlant(FVector_NetQuantize loc, FRotator rot)
{	
	UE_LOG(LogTemp, Warning, TEXT("Ok to spawn."));
	UE_LOG(LogTemp, Warning, TEXT("Has seed, spawning the plant (hidden) setting plotenum to plowed to begin the plowing animation."));
	// Spawn plant actor
	FActorSpawnParameters SpawnInfo;	
	Plant = GetWorld()->SpawnActor<APlantItem>(PlantBlueprint, loc, rot, SpawnInfo);
	Plant->MyMesh->SetVisibility(false);
	Plant->GrowthStatus = EGrowthStatus::Seedling;
	Plant->CropType = CropType;	

	// Logging info only
	FString ItemStr;
	ItemStr = UEnum::GetDisplayValueAsText(CropType).ToString();
	UE_LOG(LogTemp, Warning, TEXT("PlayerInteraction: You planted a %s."), *ItemStr);

	// Plant seed according to seed crop type
	Plant->PlotStatus = EPlotStatus::Plowed;
	Plant->InteractWithPlant();	
}

// TODO: Change "AMainCharacter* mainChar" to enemy location - Call this function BEFORE destroying the enemy actor when the player kills it. 
void UAC_PlayerInteraction::SpawnPickupItems(AMainCharacter* mainChar, ECropType cropType)
{	 
	FVector Loc;
	FRotator Rot;
	 
	// Get player location and rotation	 
	mainCharacter = Cast<AMainCharacter>(mainChar);
	mainCharacter->GetController()->GetPlayerViewPoint(Loc, Rot);	 

	/**Get ground Z location*/
		// Define the X, Y var
	FVector2D GroundXY;
	// Get the X, Y of the line trace
	GroundXY = FVector2D(Loc.X, Loc.Y);
	// Get the ground (Z) location under the line trace impact point
	GroundLoc = GetSurface(GroundXY, false);

	FVector SpawnLoc = FVector(Loc.X, Loc.Y, GroundLoc);
	 
	FActorSpawnParameters SpawnInfo;
	 
	// Reference to the crop pickup item blueprint
	ACropPickupItem* PItem = Cast<ACropPickupItem>(CropPickupBlueprint.GetDefaultObject());
	 
	// Get a random number of crops to spawn
	int RandomInt = FMath::RandRange(PItem->MinSpawnNum, PItem->MaxSpawnNum);
	 
	// TODO: Remove for prod - Just a log saying how many items were spawned
	FString IntAsString = FString::FromInt(RandomInt);
	UE_LOG(LogTemp, Warning, TEXT("Spawning %s items for pickup"), *IntAsString);
	 
	 // Loop that spawns the random number of items
	for (int i = 0; i < RandomInt; i++)
	{
		//FMath::RandInit(2);		 
		int RandomX = FMath::RandRange(PItem->MinXSpawnNum, PItem->MaxXSpawnNum);
		//FMath::RandInit(77);
		int RandomY = FMath::RandRange(PItem->MinYSpawnNum, PItem->MaxYSpawnNum);

		FString IntAsString2 = FString::FromInt(RandomX);
		FString IntAsString3 = FString::FromInt(RandomY);
		UE_LOG(LogTemp, Warning, TEXT("Random range X: %s Random Range Y: %s"), *IntAsString2, *IntAsString3);
		 

		SpawnLoc.X = SpawnLoc.X + RandomX;
		SpawnLoc.Y = SpawnLoc.Y + RandomY;


		// Spawn item and define crop type
		PickupItem = GetWorld()->SpawnActor<ACropPickupItem>(CropPickupBlueprint, SpawnLoc, Rot, SpawnInfo);
		if (PickupItem)
		{			
			PickupItem->CropType = cropType;
		}		
	}	
}
