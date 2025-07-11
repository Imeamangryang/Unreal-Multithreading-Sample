// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE5MultiThreadSampleGameMode.h"
#include "UE5MultiThreadSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5MultiThreadSampleGameMode::AUE5MultiThreadSampleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
