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
	virtual void TurnToLoc(const FVector& TargetLocation);

private:
	UPROPERTY()
	FSmoothRotator SmoothRotator;
};
