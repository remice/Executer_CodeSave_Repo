// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CanDodgeActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCanDodgeActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API ICanDodgeActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Dodge system must have system based TSet
	virtual void AddProjectileIdsToSet(const TSet<int32> NearProjectileIds) = 0;
	virtual void AddProjectileIdToSet(const int32& NearProjectileId) = 0;
};
