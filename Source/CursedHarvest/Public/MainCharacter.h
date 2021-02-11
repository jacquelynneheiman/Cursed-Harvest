// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class CURSEDHARVEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the pawn*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
 

protected:
	/** Reference to the Player Controller*/
	UPROPERTY(BlueprintReadOnly)
	class APlayerController* MainPController;	 
	 
public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(BlueprintReadOnly)
	class UAC_PlayerInteraction* InteractionComp;

	UPROPERTY(BlueprintReadOnly)
	bool bPlantingMode;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInCombat;	

public:
	
	/** Reference to Planting Reticle widget - Is to be set in BP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WPlantingReticle;
	/** Reticle ref*/
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
		UUserWidget* PlantingReticle; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	 

	UFUNCTION()
	void EKeyPressed();

	UFUNCTION()
	void LMBDown();

	UFUNCTION()
	void RMBDown();

	UFUNCTION()
	void OpenPlayerMenu();

	UFUNCTION()
	void ResetCursorToDefault();

	UFUNCTION()
	void RestoreCharMovement();

	UFUNCTION()
	void PKeyPressed();

	UFUNCTION()
	void IKeyPressed();

	UFUNCTION(meta = (DevelopmentOnly))
	void SetPlantingMode(bool bSetPlantingMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
		void AddItemToInventory();

};
