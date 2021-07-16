// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPlayerGameMode.h"
#include "MultiPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiPlayerGameMode::AMultiPlayerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
