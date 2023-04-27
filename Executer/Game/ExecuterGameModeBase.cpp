// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExecuterGameModeBase.h"
#include "Character/MainPlayerController.h"
#include "Player/ExecuterPlayerState.h"

#define PATH_BP_PLAYER_C TEXT("/Game/Character/BP_PlayerCharacter.BP_PlayerCharacter_C")

AExecuterGameModeBase::AExecuterGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DEFAULT_PAWN_CLASS(PATH_BP_PLAYER_C);

	check(DEFAULT_PAWN_CLASS.Succeeded());

	DefaultPawnClass = DEFAULT_PAWN_CLASS.Class;
	PlayerControllerClass = AMainPlayerController::StaticClass();
	PlayerStateClass = AExecuterPlayerState::StaticClass();
}
