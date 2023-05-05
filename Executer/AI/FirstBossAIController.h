// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FirstBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AFirstBossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFirstBossAIController();

	void RunAI();
	void StopAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
