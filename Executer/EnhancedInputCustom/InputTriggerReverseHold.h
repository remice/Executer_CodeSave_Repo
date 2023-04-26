// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "InputTriggerReverseHold.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UInputTriggerReverseHold : public UInputTrigger
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Trigger Settings")
	float HoldTime = 0.5f;
	
protected:
	virtual ETriggerState UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;

	float CurTime = 0.f;
};
