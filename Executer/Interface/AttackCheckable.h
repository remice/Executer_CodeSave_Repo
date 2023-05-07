// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackCheckable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackCheckable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IAttackCheckable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CheckAttachToSocket(const FName& SocketName, const FVector& PreFrameLocation, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray, TArray<TObjectPtr<AActor>> IgnoreActorArray, FHitResult& HitResult) = 0;
	virtual FVector GetLocationToSocket(const FName& SocketName) = 0;
};
