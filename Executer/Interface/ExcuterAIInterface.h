// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExcuterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExcuterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IExcuterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TurnToLoc(const FVector& TargetLocation) {}
	virtual void MoveToLoc(const FVector& TargetLocation, float InterpSpeed) {}
	virtual void MoveToLocUseMovementComponent(const FVector& TargetLocation) {}
	virtual void StopMove() {}
};
