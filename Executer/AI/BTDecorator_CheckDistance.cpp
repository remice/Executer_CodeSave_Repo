// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("CheckDistanceState");
	DistanceState = EDistanceState::Near;
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (IsValid(Blackboard) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Decorator_CheckDistance] Blackboard doesn't exist!!"));
		return bResult;
	}

	uint8 CheckEnumValue = Blackboard->GetValueAsEnum(BBPROPERTY_DistanceState);
	EDistanceState CheckEnum = StaticCast<EDistanceState>(CheckEnumValue);

	if (DistanceState == CheckEnum)
	{
		return true;
	}

	return false;
}
