// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterSkillDataAsset.generated.h"

USTRUCT()
struct FSkillAnimMontageData
{
	GENERATED_BODY()

	FSkillAnimMontageData()
	{
		bHasCurve = false;
		bCanStop = false;
		StopTime = 0.1f;
		CoolTime = 0.1f;
	}

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 bHasCurve : 1;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<class UCurveVector> CurveData;

	UPROPERTY(EditAnywhere, Category = "Animation")
	uint8 bCanStop : 1;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (ClampMin = "0.1"))
	float StopTime;

	UPROPERTY(EditAnywhere, Category = "Animation", meta = (ClampMin = "0.1"))
	float CoolTime;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API UCharacterSkillDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UCharacterSkillDataAsset();

public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FSkillAnimMontageData> SkillAnimMontageDatas;
};
