// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "Interface/PatternSpawnable.h"
#include "Interface/AnimationTaskable.h"
#include "BossBase.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API ABossBase : public ABaseEnemy, public IPatternSpawnable, public IAnimationTaskable
{
	GENERATED_BODY()
	
public:
	ABossBase();

// Interface Section
public:
	virtual void SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName) override;

	virtual void PlayAnimationFromData(const class UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc) override;
	virtual void StopAnimation() override;

protected:
	void EndAnimation(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	float AnimPlayRate = 1.0f;

	FOnEndAnimationSigniture EndAnimationDelegate;
};
