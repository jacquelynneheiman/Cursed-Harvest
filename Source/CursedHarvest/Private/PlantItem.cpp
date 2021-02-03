// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantItem.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"
#include "AC_PlayerInteraction.h"
#include "Timermanager.h"

APlantItem::APlantItem()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/StarterContent/Props/SM_Bush"));
	MyMesh->SetStaticMesh(MeshAsset.Object);

	MainCharRef = nullptr;
	
	PlowTimerLength = 2.f;
	SproutTransitionLength = 2.f;
	RipeningTransitionLength = 2.f;
	RipeTransitionLength = 2.f;	 
}

void APlantItem::BeginPlay()
{
	Super::BeginPlay();
}

void APlantItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);

	if (MainCharacter)
	{
		MainCharRef = MainCharacter;

		InteractionComp = MainCharacter->FindComponentByClass<UAC_PlayerInteraction>();
		InteractionComp->SetPlantOverlapStatus(this, true);
	}
}

void APlantItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (MainCharRef)
	{
		InteractionComp->SetPlantOverlapStatus(nullptr, false);
	}
}

void APlantItem::InteractWithPlant()
{
	 
	switch (PlotStatus)
	{
	case Unused:
	{	 
			// empty				 
	}
	break;
	case Plowed:
	{
		UE_LOG(LogTemp, Warning, TEXT("Plowing"));
		// Show plowing and seed planting animation
		//if (PlowAnimation)
		//{
		//	//PlayPlowAnimation();		
		// TODO: Change this to an anim notify in the plow animation when created so EndPlowing() gets called after plow anim finishes.
		GetWorld()->GetTimerManager().SetTimer(PlowTimer, this, &APlantItem::EndPlowing, PlowTimerLength, false);		
	}
	break;
	case Planted:
	{
		switch (GrowthStatus)
		{
		case Seedling:
		{
			FString MPath = FString(TEXT("/Game/StarterContent/Props/SM_Bush"));
			SetMesh(MPath);
			 
			// TODO: This is just to show a growth scale until actual meshes are available for each growth stage
			MyMesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
			UE_LOG(LogTemp, Warning, TEXT("Seedling planted"));		
			GetWorld()->GetTimerManager().SetTimer(SeedlingToSproutTimer, this, &APlantItem::TransitionToSprout, SproutTransitionLength, false);
		}
		break;
		case Sprout:
		{
			UE_LOG(LogTemp, Warning, TEXT("Seedling has sprouted"));

			FString MPath = FString(TEXT("/Game/StarterContent/Props/SM_Bush"));
			SetMesh(MPath);
			 
			// TODO: This is just to show a growth scale until actual meshes are available for each growth stage
			MyMesh->SetWorldScale3D(FVector(0.4f, 0.4f, 0.4f));		
			GetWorld()->GetTimerManager().SetTimer(SproutToRipeningTimer, this, &APlantItem::TransitionToRipening, RipeningTransitionLength, false);
		}
		break;
		case Ripening:
		{
			UE_LOG(LogTemp, Warning, TEXT("Plant is almost ripe!"));

			FString MPath = FString(TEXT("/Game/StarterContent/Props/SM_Bush"));
			SetMesh(MPath);
			 
			// TODO: This is just to show a growth scale until actual meshes are available for each growth stage
			MyMesh->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));
			GetWorld()->GetTimerManager().SetTimer(RipeningToRipeTimer, this, &APlantItem::TransitionToRipe, RipeTransitionLength, false);
		}
		break;
		case Ripe:
		{
			// Logging info only
			FString ItemStr;
			ItemStr = UEnum::GetDisplayValueAsText(CropType).ToString();
			UE_LOG(LogTemp, Warning, TEXT("PlantItem: Your %s is fully grown and has turned into a monster and run off! Catch and kill it to harvest it!"), *ItemStr);
			
			// Spawn combat plant
			//CombatSystem->SpawnEnemy(cropType);

			// Destroy plant actor
			this->Destroy();			 
		}
		break;
		default:
			break;
		}
	}
	break;
	default:
		break;
	}
}

 
void APlantItem::EndPlowing()
{
	UE_LOG(LogTemp, Warning, TEXT("Plowing complete"))
	// When plowing is finished, set growth status to seedling and plotstatus to planted
	PlotStatus = EPlotStatus::Planted;
	GrowthStatus = EGrowthStatus::Seedling;

	// Set plant (Mesh) visibility to true to show the new seedling
	MyMesh->SetVisibility(true);
	 PlowTimer.Invalidate();
	 // Call interact to iterate the enums according to the values set above
	InteractWithPlant();
}

void APlantItem::TransitionToSprout()
{
	GrowthStatus = EGrowthStatus::Sprout;
	SeedlingToSproutTimer.Invalidate();
	// Call interact to iterate the enums according to the values set above
	InteractWithPlant();
}

void APlantItem::TransitionToRipening()
{
	GrowthStatus = EGrowthStatus::Ripening;
	SproutToRipeningTimer.Invalidate();
	// Call interact to iterate the enums according to the values set above
	InteractWithPlant();
}

void APlantItem::TransitionToRipe()
{
	GrowthStatus = EGrowthStatus::Ripe;
	SproutToRipeningTimer.Invalidate();
	// Call interact to iterate the enums according to the values set above
	InteractWithPlant();
}


/** Set the current plant mesh according to growth stage */
void APlantItem::SetMesh(FString MeshPath)
{	 
	MeshToUse = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *MeshPath));
	MyMesh->SetStaticMesh(MeshToUse);
}

