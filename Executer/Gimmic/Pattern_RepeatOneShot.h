// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/PatternBase.h"
#include "Pattern_RepeatOneShot.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API APattern_RepeatOneShot : public APatternBase
{
	GENERATED_BODY()
	
public:
	APattern_RepeatOneShot();

protected:
	virtual void ExFire() override;
	virtual void SpawnBullets() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Pattern")
	TSubclassOf<class ABaseAttack> SpawnBulletClass;

	float ShotDelay = 99999.f;
	float CurShotDelay = 0.f;
};
