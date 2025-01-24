// Copyright Epic Games, Inc. All Rights Reserved.

#include "Match3PlayerController.h"

AMatch3PlayerController::AMatch3PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
