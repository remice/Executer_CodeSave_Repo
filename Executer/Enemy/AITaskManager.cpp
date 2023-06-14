// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITaskManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UAITaskManager::UAITaskManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	SmoothRotator = FSmoothRotator();
}

void UAITaskManager::BeginPlay()
{
	Super::BeginPlay();
	
	CallInitialize();
}

void UAITaskManager::CallInitialize()
{
	AActor* ControllingActor = GetOwner();
	ensure(IsValid(ControllingActor));

	SmoothRotator = FSmoothRotator(GetOwner()->GetWorld());

}

void UAITaskManager::TurnToLoc(const FVector& TargetLocation)
{
	AActor* ControllingActor = GetOwner();
	ensure(IsValid(ControllingActor));
	
	FVector TargetXYLocation = FVector(TargetLocation.X, TargetLocation.Y, 0);
	FVector ActorXYLocation = FVector(ControllingActor->GetActorLocation().X, ControllingActor->GetActorLocation().Y, 0);
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(ActorXYLocation, TargetXYLocation);

	FOnRotChangedSignature RotDelegate;
	RotDelegate.BindLambda(
		[&]() {
			AActor* OwningActor = GetOwner();
			if (IsValid(OwningActor))
			{
				OwningActor->SetActorRotation(SmoothRotator.CurRot);
			}
		});

	SmoothRotator.StartRotate(ControllingActor->GetActorRotation(), LookRot, RotDelegate, false);
}

// Struct Section
void FSmoothRotator::CalcSmoothRot()
{
	if (World.IsValid() == false)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		return;
	}

	if (CurRot == TargetRot)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		return;
	}

	CurRot = UKismetMathLibrary::RInterpTo_Constant(CurRot, TargetRot, World->GetDeltaSeconds(), InterpSpeed);
}

void FSmoothRotator::StartRotate(FRotator InActorRot, FRotator InTargetRot, FOnRotChangedSignature InDelegate, bool IsXYPlain)
{
	if (World.IsValid() == false)
	{
		return;
	}

	if (OnRotChanged.IsBound())
	{
		OnRotChanged.Unbind();
	}

	if (TimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	CurRot = InActorRot;
	TargetRot = InTargetRot;

	if (IsXYPlain)
	{
		TargetRot = FRotator(CurRot.Pitch, TargetRot.Yaw, CurRot.Roll);
	}

	OnRotChanged = InDelegate;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[&]() {
			CalcSmoothRot();
			OnRotChanged.ExecuteIfBound();
		});

	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.01f, true);
}