// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IRotatableObject.generated.h"

UENUM()
enum class ERotateOption : uint8
{
	Sin = 1,
	Linear,
	Default
};

UENUM()
enum class ERotateDirection : uint8
{
	Roll = 0,
	Yaw,
	Pitch
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIRotatableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IIRotatableObject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RelativeRotate(float SpanTime) = 0;
};
