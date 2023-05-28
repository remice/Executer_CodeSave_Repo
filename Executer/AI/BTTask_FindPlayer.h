// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTTask_FindPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindPlayer();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Collision", meta = (AllowPrivateAccess = "true"))
	float CheckRadius;
};
