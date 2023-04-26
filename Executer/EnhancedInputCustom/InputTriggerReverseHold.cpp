// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTriggerReverseHold.h"

ETriggerState UInputTriggerReverseHold::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (IsActuated(ModifiedValue) == false && IsActuated(LastValue) == true)
	{
		CurTime = 0.f;

		return ETriggerState::None;
	}

	if (CurTime < HoldTime)
	{
		CurTime += DeltaTime;

		return ETriggerState::Ongoing;
	}

	return ETriggerState::None;
}
