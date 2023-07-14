// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TutoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UTutoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTutoAnimInstance();
	
public:
	UPROPERTY(BlueprintReadOnly, Category="Animation")
	uint8 bOnDead : 1;
};
