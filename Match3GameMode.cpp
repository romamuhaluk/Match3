// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3GameMode.h"
#include "Match3PlayerController.h"
#include "Match3Pawn.h"

AMatch3GameMode::AMatch3GameMode()
{
	// no pawn by default
	DefaultPawnClass = AMatch3Pawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMatch3PlayerController::StaticClass();
}
