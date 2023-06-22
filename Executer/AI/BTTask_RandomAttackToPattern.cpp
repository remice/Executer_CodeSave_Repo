// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RandomAttackToPattern.h"

#include "AIController.h"
#include "Interface/AnimationTaskable.h"

UBTTask_RandomAttackToPattern::UBTTask_RandomAttackToPattern()
{
	NodeName = TEXT("RandomAttack");
}

EBTNodeResult::Type UBTTask_RandomAttackToPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	float SumWeight = 0;
	for (auto RandomAttackData : RandomAttackDatas)
	{
		SumWeight += RandomAttackData.Weight;
	}

	float RandomValue = FMath::RandRange(0.f, SumWeight);
	SumWeight = 0;
	for (auto RandomAttackData : RandomAttackDatas)
	{
		if (SumWeight <= RandomValue && RandomValue < SumWeight + RandomAttackData.Weight)
		{
			FOnEndAnimationSigniture OnTaskEnd;
			OnTaskEnd.BindLambda(
				[&]()
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}
			);
			TaskPawn->PlayAnimationFromData(RandomAttackData.BossPatternData, OnTaskEnd);

			return EBTNodeResult::InProgress;
		}

		SumWeight += RandomAttackData.Weight;
	}
	return EBTNodeResult::Failed;
}
