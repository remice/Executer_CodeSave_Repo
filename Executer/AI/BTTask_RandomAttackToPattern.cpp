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
			if (ExecutePattern(OwnerComp, TaskPawn, RandomAttackData.BossPatternData))
			{
				return EBTNodeResult::InProgress;
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}

		SumWeight += RandomAttackData.Weight;
	}
	return EBTNodeResult::Failed;
}

bool UBTTask_RandomAttackToPattern::ExecutePattern(UBehaviorTreeComponent& OwnerComp, class IAnimationTaskable* TaskPawn, UBossPatternData* InPatternData)
{
	if (InPatternData == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RandomAttackToPattern] Pattern Data is nullptr"));
		return false;
	}
	FOnEndAnimationSigniture OnTaskEnd;
	OnTaskEnd.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);
	TaskPawn->PlayAnimationFromData(InPatternData, OnTaskEnd);

	return true;
}
