// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FirstBossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#define PATH_BB TEXT("/Script/AIModule.BlackboardData'/Game/Character/Enemy/BB_Gideon.BB_Gideon'")
#define PATH_BT TEXT("/Script/AIModule.BehaviorTree'/Game/Character/Enemy/BT_Gideon.BT_Gideon'")

AFirstBossAIController::AFirstBossAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(PATH_BB);
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(PATH_BT);

	ensure(BBAssetRef.Succeeded());
	ensure(BTAssetRef.Succeeded());

	BBAsset = BBAssetRef.Object;
	BTAsset = BTAssetRef.Object;
}

void AFirstBossAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		//Blackboard->SetValueAsVector(, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AFirstBossAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AFirstBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}
