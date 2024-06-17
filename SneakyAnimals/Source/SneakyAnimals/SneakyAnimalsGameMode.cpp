// Copyright Epic Games, Inc. All Rights Reserved.

#include "SneakyAnimalsGameMode.h"
#include "SneakyAnimalsCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASneakyAnimalsGameMode::ASneakyAnimalsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
