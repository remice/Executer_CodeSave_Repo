// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BossBase.h"
#include "Interface/AIManagerGettable.h"
#include "Interface/ExcuterAIInterface.h"
#include "AITaskManager.h"

#include "FirstBoss.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AFirstBoss : public ABossBase, public IAIManagerGettable, public IExcuterAIInterface
{
	GENERATED_BODY()
	
public:
	AFirstBoss();

	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual UObject* GetManager() override { return Cast<UObject>(AITaskManager); }

	virtual FRotator GetGroundMoveRot();

protected:
	virtual void StartCurveMove(class UCurveVector* CurveData);

private:
	void PlayMontage(class UAnimMontage* Montage, bool IsPrimitive = true);
	
	// ExecuterAIInterface Section
	virtual void TurnToLoc(const FVector& TargetLocation) override;
	virtual void MoveToLoc(const FVector& TargetLocation, float InterpSpeed) override;
	virtual void MoveToLocUseMovementComponent(const FVector& TargetLocation) override;
	virtual void StopMove() override;
	// Section End

protected:
	UPROPERTY()
	TObjectPtr<class UAITaskManager> AITaskManager;

	UPROPERTY()
	TObjectPtr<class UHitMontageDataAsset> HitMontages;

	UPROPERTY()
	TObjectPtr<class UAnimInstance> AnimInstance;

	FVector PrePosition = FVector::ZeroVector;
};
