// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FacePattern.h"
#include "BlackboardValueName.h"

UBTTask_FacePattern::UBTTask_FacePattern()
{
	NodeName = TEXT("Face Pattern");
}

EBTNodeResult::Type UBTTask_FacePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	check(BlackboardComp);

	if (BlackboardComp->GetValueAsBool(BBPROPERTY_bIsFace)) return EBTNodeResult::Failed;

	EBTNodeResult::Type SaveResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	BlackboardComp->SetValueAsBool(BBPROPERTY_bIsFace, true);

	return SaveResult;
}
