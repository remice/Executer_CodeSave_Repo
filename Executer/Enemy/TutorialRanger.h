// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "TutorialRanger.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API ATutorialRanger : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ATutorialRanger();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY()
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<class UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, Category=Pattern)
	TArray<TSubclassOf<class APatternBase>> PatternClass;

	UPROPERTY()
	TObjectPtr<class APatternBase> PatternManager;

	UPROPERTY(EditAnywhere, Category=Pattern)
	TArray<float> DelayArray;

	UPROPERTY()
	int32 DelayIndex;

	float CurDelay;

	uint8 bOnPattern : 1;

protected:
	UFUNCTION()
	void ExNextPattern();

	UFUNCTION()
	void SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass);
};
