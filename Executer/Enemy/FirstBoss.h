// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BossBase.h"
#include "Interface/AIManagerGettable.h"
#include "FirstBoss.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AFirstBoss : public ABossBase, public IAIManagerGettable
{
	GENERATED_BODY()
	
public:
	AFirstBoss();

	virtual UObject* GetManager() { return AITaskManager; }

protected:
	UPROPERTY()
	TObjectPtr<class UAITaskManager> AITaskManager;
	
};
