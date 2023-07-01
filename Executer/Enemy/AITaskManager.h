// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Initializable.h"
#include "AITaskManager.generated.h"

DECLARE_DELEGATE(FOnRotChangedSignature)

USTRUCT()
struct FSmoothRotator
{
	GENERATED_BODY()

public:
	FSmoothRotator() : World(nullptr) {}
	FSmoothRotator(UWorld* InWorld) : World(InWorld) {}

public:
	void CalcSmoothRot();
	void StartRotate(FRotator InActorRot, FRotator InTargetRot, FOnRotChangedSignature InDelegate, bool IsXYPlain = true);

public:
	FOnRotChangedSignature OnRotChanged;

	float InterpSpeed = 30;

public:
	FRotator TargetRot = FRotator::ZeroRotator;
	FRotator CurRot = FRotator::ZeroRotator;

	FTimerHandle TimerHandle;

	UPROPERTY()
	TWeakObjectPtr<UWorld> World;
};

DECLARE_DELEGATE(FOnLocChangedSignature)

USTRUCT()
struct FSmoothMover
{
	GENERATED_BODY()

	public:
	FSmoothMover() : World(nullptr) {}
	FSmoothMover(UWorld* InWorld) : World(InWorld) {}

public:
	void CalcSmoothMove();
	void StartMove(FVector InActorLoc, FVector InTargetLoc, FOnLocChangedSignature InDelegate, bool IsXYPlain = true);

public:
	FOnLocChangedSignature OnLocChanged;

	float InterpSpeed = 30;

public:
	FVector TargetLoc = FVector::ZeroVector;
	FVector CurLoc = FVector::ZeroVector;

	FTimerHandle TimerHandle;

	UPROPERTY()
	TWeakObjectPtr<UWorld> World;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UAITaskManager : public UActorComponent, public IInitializable
{
	GENERATED_BODY()

public:	
	UAITaskManager();

// Initializable interface Section
public:
	virtual void CallInitialize() override;
// Section End

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TurnToLoc(const FVector& TargetLocation, float InterpSpeed);
	virtual void MoveToLoc(const FVector& TargetLocation, float InterpSpeed);
	virtual void MoveToLocUseMovementComponent(const FVector& TargetLocation);
	virtual void StopMove();
	FORCEINLINE void SetEnalbeComponent(bool IsEnable) { bOnEnable = IsEnable; }

private:
	FTimerHandle TimerHandler;
	FVector MoveTarget;
	FVector MoveDir;
	float ArriveChecker;
	uint8 bOnEnable : 1;

	UPROPERTY()
	FSmoothRotator SmoothRotator;

	UPROPERTY()
	FSmoothMover SmoothMover;
};
