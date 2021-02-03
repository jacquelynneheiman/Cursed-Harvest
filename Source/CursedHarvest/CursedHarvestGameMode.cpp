// Copyright Epic Games, Inc. All Rights Reserved.

#include "CursedHarvestGameMode.h"
#include "CursedHarvestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACursedHarvestGameMode::ACursedHarvestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
