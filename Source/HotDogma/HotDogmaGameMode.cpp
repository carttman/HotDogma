// Copyright Epic Games, Inc. All Rights Reserved.

#include "HotDogmaGameMode.h"
#include "HotDogmaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHotDogmaGameMode::AHotDogmaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
