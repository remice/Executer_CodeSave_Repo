// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_AttackToPattern.h"
#include "BTTask_FacePattern.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_FacePattern : public UBTTask_AttackToPattern
{
	GENERATED_BODY()
	
public:
	UBTTask_FacePattern();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
