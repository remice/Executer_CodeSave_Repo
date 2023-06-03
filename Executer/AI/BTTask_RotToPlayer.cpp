// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_RotToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/AIManagerGettable.h"
#include "Enemy/AITaskManager.h"
#include "BlackboardValueName.h"

UBTTask_RotToPlayer::UBTTask_RotToPlayer()
{
	NodeName = TEXT("RotToPlayer");
}

EBTNodeResult::Type UBTTask_RotToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get controlling pawn
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RotToPlayer] AI doesn't have owner pawn"));
		return EBTNodeResult::Failed;
	}

	IAIManagerGettable* HasAIManagerActor = Cast<IAIManagerGettable>(ControllingPawn);
	if (HasAIManagerActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RotToPlayer] AI pawn doesn't implement IAIManagerGettable"));
		return EBTNodeResult::Failed;
	}
	
	UObject* AIManagerObject = HasAIManagerActor->GetManager();
	if (IsValid(AIManagerObject) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RotToPlayer] Can't get manager"));
		return EBTNodeResult::Failed;
	}

	UAITaskManager* AIManager = Cast<UAITaskManager>(AIManagerObject);
	if (AIManager == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RotToPlayer] Cast fail to AIManager"));
		return EBTNodeResult::Failed;
	}

	// Apply actor into blackboard
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_RotToPlayer] AI doesn't have blackboard"));
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = BlackboardComp->GetValueAsObject(BBPROPERTY_PlayerPawn);
	if (IsValid(TargetObject) == false) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (IsValid(TargetActor) == false) return EBTNodeResult::Failed;

	AIManager->TurnToLoc(TargetActor->GetActorLocation());

	return EBTNodeResult::Succeeded;
}
