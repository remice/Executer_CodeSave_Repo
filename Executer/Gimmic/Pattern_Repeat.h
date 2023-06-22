// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/PatternBase.h"
#include "Pattern_Repeat.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API APattern_Repeat : public APatternBase
{
	GENERATED_BODY()
	
public:
	APattern_Repeat();

	virtual void ExFire() override;
	virtual void SpawnBullets() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Pattern")
	TSubclassOf<class AAttackBase> SpawnBulletClass;

	UPROPERTY(EditAnywhere, Category="Pattern")
	float NextDelay;
	float CurDelay;
};
