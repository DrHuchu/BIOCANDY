// Copyright Epic Games, Inc. All Rights Reserved.

#include "BIOCANDYGameMode.h"
#include "BIOCANDYCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABIOCANDYGameMode::ABIOCANDYGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
