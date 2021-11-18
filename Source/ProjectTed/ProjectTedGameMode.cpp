// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectTedGameMode.h"
#include "ProjectTedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectTedGameMode::AProjectTedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
