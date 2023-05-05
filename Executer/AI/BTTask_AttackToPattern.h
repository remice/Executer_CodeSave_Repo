// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AttackToPattern.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_AttackToPattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_AttackToPattern();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Task")
	TObjectPtr<class UBossPatternData> BossPatternData;
};
