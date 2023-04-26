// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExecuterGameModeBase.h"
#include "Character/PlayerCharacter.h"
#include "Character/MainPlayerController.h"
#include "Player/ExecuterPlayerState.h"

AExecuterGameModeBase::AExecuterGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMainPlayerController::StaticClass();
	PlayerStateClass = AExecuterPlayerState::StaticClass();
}
