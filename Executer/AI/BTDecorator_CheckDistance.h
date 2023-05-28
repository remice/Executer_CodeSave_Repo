// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BlackboardValueName.h"
#include "BTDecorator_CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBTDecorator_CheckDistance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckDistance();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Enum", meta = (AllowPrivateAccess = "true"))
	EDistanceState DistanceState;
};
