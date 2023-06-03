// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RotToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_RotToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RotToPlayer();
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
