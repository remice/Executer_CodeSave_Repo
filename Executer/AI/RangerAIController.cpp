// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RangerAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#define PATH_BB TEXT("/Script/AIModule.BlackboardData'/Game/Character/Enemy/BB_TutoMinion_Range.BB_TutoMinion_Range'")
#define PATH_BT TEXT("/Script/AIModule.BehaviorTree'/Game/Character/Enemy/BT_TutoMinion_Range.BT_TutoMinion_Range'")

ARangerAIController::ARangerAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(PATH_BB);
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(PATH_BT);

	ensure(BBAssetRef.Succeeded());
	ensure(BTAssetRef.Succeeded());

	BBAsset = BBAssetRef.Object;
	BTAsset = BTAssetRef.Object;
}

void ARangerAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void ARangerAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void ARangerAIController::RestartAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->RestartTree();
	}
}

void ARangerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
