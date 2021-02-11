// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
 
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	if (WInventory)
	{
		Inventory = CreateWidget<UUserWidget>(this, WInventory);
		if (Inventory)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Create"));
			Inventory->AddToViewport();
			Inventory->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AMainPlayerController::DisplayInventory_Implementation()
{
	if (Inventory)
	{
		bInventoryVisible = true;
		Inventory->SetVisibility(ESlateVisibility::Visible);

		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);

		bShowMouseCursor = true;
	}
}

void AMainPlayerController::HideInventory_Implementation()
{
	if (Inventory)
	{
		Inventory->SetVisibility(ESlateVisibility::Hidden);
		bShowMouseCursor = false;
		bInventoryVisible = false;

		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
	}
}

void AMainPlayerController::ToggleInventory()
{
	if (bInventoryVisible)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hide"));
		HideInventory();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Display"));
		DisplayInventory();
	}
}

UUserWidget* AMainPlayerController::GetInventory()
{
	return Inventory;
}
