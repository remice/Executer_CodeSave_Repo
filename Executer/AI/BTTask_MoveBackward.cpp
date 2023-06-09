// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveBackward.h"

#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/ExcuterAIInterface.h"

UBTTask_MoveBackward::UBTTask_MoveBackward()
{
	NodeName = TEXT("MoveBackward");
	MinDistance = 300;
	MaxDistance = 500;
	InterpSpeed = 30;
}

EBTNodeResult::Type UBTTask_MoveBackward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SaveResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get controlling pawn
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_MoveFront] AI doesn't have owner pawn"));
		return EBTNodeResult::Failed;
	}

	// Apply actor into blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_FindPlayer] AI doesn't have blackboard"));
		return EBTNodeResult::Failed;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActorArray;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	FHitResult HitResult;
	bool IsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(ControllingPawn->GetWorld(), ControllingPawn->GetActorLocation(),
		ControllingPawn->GetActorLocation() + FVector(0, 0, -150), ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::ForDuration, HitResult, true);

	if (IsCollide == false)
	{
		return EBTNodeResult::Failed;
	}

	for (int i = 0; i < 10; i++)
	{
		float RandValue = FMath::RandRange(MinDistance, MaxDistance);

		FVector TargetLocation = ControllingPawn->GetActorLocation() + ControllingPawn->GetActorForwardVector() * -RandValue;

		if (PerformMove(ControllingPawn, ControllingPawn->GetActorLocation(), TargetLocation)) return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

bool UBTTask_MoveBackward::PerformMove(APawn* ControllingPawn, FVector ActorLocation, FVector TargetLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActorArray;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	FHitResult HitResult;
	bool IsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(ControllingPawn->GetWorld(), ActorLocation + FVector(0, 0, 50), 
		TargetLocation + FVector(0, 0, 50), ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::ForDuration, HitResult, true);

	if (IsCollide)
	{
		return false;
	}

	IsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(ControllingPawn->GetWorld(), TargetLocation + FVector(0, 0, 100),
		TargetLocation + FVector(0, 0, -600), ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::ForDuration, HitResult, true);

	if (IsCollide)
	{
		IExcuterAIInterface* AIPawn = Cast<IExcuterAIInterface>(ControllingPawn);
		if (AIPawn)
		{
			AIPawn->MoveToLoc(HitResult.ImpactPoint + FVector(0, 0, 88.f), InterpSpeed);
			return true;
		}
	}
	return false;
}
