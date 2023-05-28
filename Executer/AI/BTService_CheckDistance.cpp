// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BlackboardValueName.h"

UBTService_CheckDistance::UBTService_CheckDistance()
{
	NearDistance = 500.f;
	MiddleDistance = 1000.f;

	Interval = 0.5f;
	NodeName = TEXT("CheckDistance");
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(ControllingPawn) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTService_CheckDistance] Controlling pawn doesn't exist!!"));
		return;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (IsValid(Blackboard) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTService_CheckDistance] Blackboard doesn't exist!!"));
		return;
	}

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(BBPROPERTY_PlayerPawn));
	if (IsValid(Target) == false)
	{
		return;
	}

	float Distance = (ControllingPawn->GetActorLocation() - Target->GetActorLocation()).Size();

	if (Distance < NearDistance)
	{
		Blackboard->SetValueAsEnum(BBPROPERTY_DistanceState, StaticCast<uint8>(EDistanceState::Near));
		return;
	}
	else if (Distance < MiddleDistance)
	{
		Blackboard->SetValueAsEnum(BBPROPERTY_DistanceState, StaticCast<uint8>(EDistanceState::Middle));
		return;
	}
	
	Blackboard->SetValueAsEnum(BBPROPERTY_DistanceState, StaticCast<uint8>(EDistanceState::Far));
	return;
}
