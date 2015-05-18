// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "StealthGame.h"
#include "StealthGameGameMode.h"
#include "StealthGamePlayerController.h"
#include "StealthGameCharacter.h"

AStealthGameGameMode::AStealthGameGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = AStealthGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}