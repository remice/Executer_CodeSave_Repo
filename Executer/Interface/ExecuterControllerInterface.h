// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExecuterControllerInterface.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	Q = 0,
	E,
	R,
	T,
	Special
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExecuterControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IExecuterControllerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnBossHpBar() {}
	virtual void OnSkillCooldownUI(ESkillType SkillType, float Time) {}
	virtual void OnChangedSkillUI(ESkillType SkillType, const UTexture2D* Icon) {}
};
