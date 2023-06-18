// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveBackward.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_MoveBackward : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveBackward();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	bool PerformMove(APawn* ControllingPawn, FVector ActorLocation, FVector TargetLocation);

public:
	UPROPERTY(EditAnywhere, Category = "Move")
	float MinDistance;

	UPROPERTY(EditAnywhere, Category = "Move")
	float MaxDistance;

	UPROPERTY(EditAnywhere, Category = "Move")
	float InterpSpeed;
};
