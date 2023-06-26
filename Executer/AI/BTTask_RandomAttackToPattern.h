// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RandomAttackToPattern.generated.h"

USTRUCT()
struct FRandomAttackData 
{
	GENERATED_BODY()
	
public:
	FRandomAttackData() : BossPatternData(nullptr), Weight(1) {}

public:
	UPROPERTY(EditAnywhere, Category = "Task")
	TObjectPtr<class UBossPatternData> BossPatternData;

	UPROPERTY(EditAnywhere, Category = "Task", meta = (ClampMin = "1"))
	float Weight;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_RandomAttackToPattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomAttackToPattern();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual bool ExecutePattern(UBehaviorTreeComponent& OwnerComp, class IAnimationTaskable* TaskPawn, class UBossPatternData* InPatternData);

public:
	UPROPERTY(EditAnywhere, Category = "Task")
	TArray<FRandomAttackData> RandomAttackDatas;
};
