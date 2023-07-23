// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITaskManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Pawn.h"
#include "FirstBoss.h"

UAITaskManager::UAITaskManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	SmoothRotator = FSmoothRotator();
	SmoothMover = FSmoothMover();
	ArriveChecker = 10.f;
	bOnEnable = true;
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
	SmoothMover = FSmoothMover(GetOwner()->GetWorld());
}

void UAITaskManager::TurnToLoc(const FVector& TargetLocation, float InterpSpeed)
{
	if (bOnEnable == false) return;

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

	SmoothRotator.InterpSpeed = InterpSpeed;
	SmoothRotator.StartRotate(ControllingActor->GetActorRotation(), LookRot, RotDelegate, false);
}

void UAITaskManager::MoveToLoc(const FVector& TargetLocation, float InterpSpeed)
{
	if (bOnEnable == false) return;

	AActor* ControllingActor = GetOwner();
	ensure(IsValid(ControllingActor));

	FOnLocChangedSignature MoveDelegate;
	MoveDelegate.BindLambda(
		[&]() {
			AFirstBoss* OwningActor = Cast<AFirstBoss>(GetOwner());
			if (IsValid(OwningActor))
			{
				SmoothMover.CurLoc = OwningActor->SetActorGroundLocation(SmoothMover.CurLoc);
			}});

	SmoothMover.InterpSpeed = InterpSpeed;
	SmoothMover.StartMove(ControllingActor->GetActorLocation(), TargetLocation, MoveDelegate, false);
}

void UAITaskManager::MoveToLocUseMovementComponent(const FVector& TargetLocation)
{
	if (bOnEnable == false) return;

	if (TimerHandler.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	}

	MoveTarget = TargetLocation;

	FVector Direction = MoveTarget - GetOwner()->GetActorLocation();
	MoveDir = Direction.GetSafeNormal2D(0.001f);

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda(
		[&]() {
			APawn* OwningPawn = Cast<APawn>(GetOwner());
			if (OwningPawn)
			{
				if (FVector::Distance(OwningPawn->GetActorLocation(), MoveTarget) < ArriveChecker)
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
					return;
				}
				
				if (MoveDir.Dot(MoveTarget - GetOwner()->GetActorLocation()) < 0)
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
					return;
				}
			}
		}
	), 0.01f, true);
}

void UAITaskManager::StopMove()
{
	if (GetWorld() == false) return;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	GetWorld()->GetTimerManager().ClearTimer(SmoothMover.TimerHandle);
}

// Struct Section
void FSmoothRotator::CalcSmoothRot()
{
	if (World.IsValid() == false)
	{
		return;
	}

	if (CurRot == TargetRot)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		return;
	}

	CurRot = UKismetMathLibrary::RInterpTo_Constant(CurRot, TargetRot, World->GetDeltaSeconds(), InterpSpeed);
	OnRotChanged.ExecuteIfBound();
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
		});

	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.01f, true);
}

void FSmoothMover::CalcSmoothMove()
{
	if (World.IsValid() == false)
	{
		return;
	}

	if (FVector::Distance(TargetLoc, CurLoc) <= 10.f)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		return;
	}

	CurLoc = UKismetMathLibrary::VInterpTo_Constant(CurLoc, TargetLoc, World->GetDeltaSeconds(), InterpSpeed);
	OnLocChanged.ExecuteIfBound();
}

void FSmoothMover::StartMove(FVector InActorLoc, FVector InTargetLoc, FOnLocChangedSignature InDelegate, bool IsXYPlain)
{
	if (World.IsValid() == false)
	{
		return;
	}

	if (OnLocChanged.IsBound())
	{
		OnLocChanged.Unbind();
	}

	if (TimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	CurLoc = InActorLoc;
	TargetLoc = InTargetLoc;

	if (IsXYPlain)
	{
		TargetLoc = FVector(TargetLoc.X, TargetLoc.Y, CurLoc.Z);
	}

	OnLocChanged = InDelegate;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[&]() {
			CalcSmoothMove();
		});

	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.01f, true);
}
