// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PatternSpawnable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPatternSpawnable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IPatternSpawnable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SpawnPatternManager(TSubclassOf<class APatternBase> NewPatternClass, FName SocketName) = 0;
};
