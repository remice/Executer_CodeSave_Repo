// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StunMontageDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UStunMontageDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UStunMontageDataAsset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> StunMontage;

	UPROPERTY(EditAnywhere, Category = Name)
	FString EndSectionName;
};
