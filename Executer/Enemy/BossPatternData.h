// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BossPatternData.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UBossPatternData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UBossPatternData();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> LinkAnimationMontage;

	UPROPERTY(EditAnywhere, Category = Animation, meta = (ClampMin = "0.1"))
	float PlayRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	TObjectPtr<class UCurveVector> MoveCurve;

	UPROPERTY(EditAnywhere, Category = Movement)
	uint8 bOnMove : 1;
};
