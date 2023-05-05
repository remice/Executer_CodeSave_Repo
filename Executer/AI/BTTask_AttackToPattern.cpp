// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackToPattern.h"
#include "AIController.h"
#include "Interface/AnimationTaskable.h"

UBTTask_AttackToPattern::UBTTask_AttackToPattern()
{
}

EBTNodeResult::Type UBTTask_AttackToPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAnimationTaskable* TaskPawn = Cast<IAnimationTaskable>(ControllingPawn);
	if (TaskPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FOnEndAnimationSigniture OnTaskEnd;
	OnTaskEnd.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	TaskPawn->PlayAnimationFromData(BossPatternData, OnTaskEnd);

    return EBTNodeResult::InProgress;
}
