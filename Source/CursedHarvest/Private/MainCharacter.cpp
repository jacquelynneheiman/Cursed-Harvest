// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainPlayerController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AC_PlayerInteraction.h"
#include "PlantItem.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "CropPickupItem.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the Controller rotates. Let it just affect the camera. Decouples the mouse movement from the character.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...(W,S,A,D, etc) but camera remains where we place it with mouse
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm	
	 
	bIsInCombat = false;
	bPlantingMode = false;	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();	
	MainPController = UGameplayStatics::GetPlayerController(this, 0);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("EKey", IE_Pressed, this, &AMainCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("LMBDown", IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction("RMBDown", IE_Pressed, this, &AMainCharacter::RMBDown);
	PlayerInputComponent->BindAction("PKey", IE_Pressed, this, &AMainCharacter::PKeyPressed);
	PlayerInputComponent->BindAction("IKey", IE_Pressed, this, &AMainCharacter::IKeyPressed);
}

void AMainCharacter::RestoreCharMovement()
{
	if (bPlantingMode == true)
	{				
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->SetMovementMode(EMovementMode::MOVE_Walking);

		// Give player back control of the camera rotation
		CameraBoom->TargetArmLength = 300.f;
		CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, 0.f), false, 0);
		CameraBoom->bUsePawnControlRotation = true;
	}
}

void AMainCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// Find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// Add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// Player world interaction keypress
void AMainCharacter::EKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed E Key"));

	// Make sure player isn't in combat before opening menu
	if (!bIsInCombat)
	{
		// Interact component reference
		InteractionComp = FindComponentByClass<UAC_PlayerInteraction>();

		// Is player overlapping a plant?
		if (InteractionComp->OverlappedPlantActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Reporting as overlapped"));
			// Perform menu action regarding the overlapped plant (open stats on the plant, etc)
		}
		else
		{
			OpenPlayerMenu();			
		}
	}
}

void AMainCharacter::LMBDown()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMB DOWN"));
	if (bPlantingMode == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Yep, in planting mode, try to plant something"));
		// See if the player can plant here
		InteractionComp->CheckForValidPlantSpot(this);
	}
}

void AMainCharacter::PKeyPressed()
{	 
	UE_LOG(LogTemp, Warning, TEXT("Pressed P Key"));
	// Interact component reference
	InteractionComp = FindComponentByClass<UAC_PlayerInteraction>();
	// TODO: The vars and function below should be called/set from the Combat system when the enemy is dead
	ECropType CropType;
	CropType = ECropType::Strawberry;
	InteractionComp->SpawnPickupItems(this, CropType);
}

void AMainCharacter::IKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Pressed I Key"));
	AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(MainPController);
	MainPlayerController->ToggleInventory();
}

void AMainCharacter::RMBDown()
{
	UE_LOG(LogTemp, Warning, TEXT("RMB Down"));
	ResetCursorToDefault();
	RestoreCharMovement();
	SetPlantingMode(false);
}

void AMainCharacter::OpenPlayerMenu()
{
	// TODO: Bring up player menu here so they can choose which seed to try and plant
				// TODO: DEV ONLY FUNCTION - Remove when menu has been created			 
	SetPlantingMode(true);
	// Menu->OpenMenu();

	if (MainPController)
	{
		MainPController->SetInputMode(FInputModeGameAndUI());
		/** Modify character movement while menu is open*/
		MainPController->bShowMouseCursor = true;
		UPawnMovementComponent* MovementComp;
		MovementComp = this->GetMovementComponent();
		// Stop any current player movement
		MovementComp->StopMovementImmediately();
		// Prevent character from moving while interacting with world/menu
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		Movement->SetMovementMode(EMovementMode::MOVE_None);

		// Lock camera position high above the player for planting mode
		CameraBoom->TargetArmLength = 400.f;
		CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f), false, 0);
		CameraBoom->bUsePawnControlRotation = false;

		// Assign 'Crosshairs' mouse pointer to use the Planting Reticle widget in BP if (WPlantingReticle)
		{
			PlantingReticle = CreateWidget<UUserWidget>(MainPController, WPlantingReticle);
			if (PlantingReticle)
			{
				// Changes mouse cursor to a targeting reticle
				MainPController = UGameplayStatics::GetPlayerController(this, 0);
				MainPController->CurrentMouseCursor = EMouseCursor::Crosshairs;
				// (Overrides the built-in Crosshairs cursor)
				MainPController->SetMouseCursorWidget(EMouseCursor::Crosshairs, PlantingReticle);
			}
		}
	}
}

void AMainCharacter::ResetCursorToDefault()
{
	/** If player is in planting mode,
		set input mode to GameMode
		reset character movement*/
	if (bPlantingMode == true)
	{
		MainPController = UGameplayStatics::GetPlayerController(this, 0);
		MainPController->SetMouseCursorWidget(EMouseCursor::Default, nullptr);
		//MainPlayerController->CurrentMouseCursor = EMouseCursor::Default; NOTE: This was causing an error. I thought it was necessary to set mouse cursor

		MainPController->SetInputMode(FInputModeGameOnly());
		MainPController->bShowMouseCursor = false;
	}	
}

// DEV ONLY - Won't be called in shipping
void AMainCharacter::SetPlantingMode(bool bSetPlantingMode)
{
	bPlantingMode = bSetPlantingMode;
}

void AMainCharacter::AddItemToInventory_Implementation()
{
	// Code is in BP at this time.
}
