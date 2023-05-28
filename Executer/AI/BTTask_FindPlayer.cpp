// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPlayer.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BlackboardValueName.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_FindPlayer::UBTTask_FindPlayer()
{
	NodeName = TEXT("FindPlayer");
	CheckRadius = 1000.f;
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Get controlling pawn
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BTTask_FindPlayer] AI doesn't have owner pawn"));
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

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult HitResult;
	bool OnCollide = UKismetSystemLibrary::SphereTraceSingleForObjects(ControllingPawn->GetWorld(), ControllingPawn->GetActorLocation(), ControllingPawn->GetActorLocation(),
		CheckRadius, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, HitResult, true);

	if (OnCollide == false)
	{
		return EBTNodeResult::Failed;
	}

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	for (int ZMultiplier = -1; ZMultiplier <= 1; ZMultiplier++)
	{
		FHitResult FindResult;
		bool OnFindPawn = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			ControllingPawn->GetActorLocation() + FVector(0, 0, 44.f),
			HitResult.GetActor()->GetActorLocation() + (FVector(0, 0, 44.f) * ZMultiplier),
			ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, FindResult, true);

		if (OnFindPawn == false)
		{
			continue;
		}

		AActor* HitActor = FindResult.GetActor();
		APawn* HitPawn = Cast<APawn>(HitActor);
		if (HitPawn == false)
		{
			continue;
		}

		BlackboardComp->SetValueAsObject(BBPROPERTY_PlayerPawn, FindResult.GetActor());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
