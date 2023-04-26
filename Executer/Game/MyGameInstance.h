// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetMapBoss(const TObjectPtr<class ABaseEnemy> InMapBoss) { MapBoss = InMapBoss; }
	class ABaseEnemy* GetMapBoss() const { return MapBoss; }

private:
	UPROPERTY()
	TObjectPtr<class ABaseEnemy> MapBoss;
};
