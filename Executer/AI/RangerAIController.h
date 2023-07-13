// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RangerAIController.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API ARangerAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARangerAIController();

	void RunAI();
	void StopAI();
	void RestartAI();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
