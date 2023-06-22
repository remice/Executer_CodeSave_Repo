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

protected:
	virtual void Tick(float DeltaTime) override;

// Interface Section
public:
	virtual void SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName, bool bAttachLocation, bool bAttachRotation) override;

	virtual bool PlayAnimationFromData(const class UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc) override;
	virtual void StopAnimation() override;

protected:
	void EndAnimation(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

// Curve Movement Section
protected:
	virtual void StartCurveMove(class UCurveVector* CurveData);
	void RunCurveMove();
	void EndCurveMove();

protected:
	float AnimPlayRate = 1.0f;

	FOnEndAnimationSigniture EndAnimationDelegate;

// Curve Movement Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<class UCurveVector> AnimCurveData;

	uint8 bOnCurve : 1;
	float PlayRate = 1.f;
	FVector SaveLocation;
	FVector SaveForwardVector;

protected:
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};
