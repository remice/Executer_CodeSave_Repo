// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AnimationTaskable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAnimationTaskable : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FOnEndAnimationSigniture)

/**
 * 
 */
class EXECUTER_API IAnimationTaskable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PlayAnimationFromData(const class UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc) = 0;
	virtual void StopAnimation() = 0;
};
