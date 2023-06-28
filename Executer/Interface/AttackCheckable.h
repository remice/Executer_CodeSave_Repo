// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackCheckable.generated.h"

UENUM()
enum class ECheckType : uint8
{
	Sphere = 0,
	Box,
};

USTRUCT()
struct FCheckValue
{
	GENERATED_BODY()
	
public:
	FCheckValue() : bDebugTrace(false) {}

	void SetSphereValue(float InRadius) { SphereRadius = InRadius; }
	void SetBoxValue(float InWidth, float InHeight) { BoxWidth = InWidth; BoxHeight = InHeight; }

public:
	float SphereRadius = 1;
	float BoxWidth = 1;
	float BoxHeight = 1;
	uint8 bDebugTrace : 1;
};

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
	virtual bool CheckAttachToSocket(const FName& SocketName, ECheckType CheckType, FCheckValue CheckValue, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray, TArray<TObjectPtr<AActor>> IgnoreActorArray, FHitResult& HitResult) { return false; }
	virtual FVector GetLocationToSocket(const FName& SocketName) = 0;
};
