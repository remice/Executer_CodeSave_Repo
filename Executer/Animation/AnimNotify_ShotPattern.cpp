// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShotPattern.h"
#include "Gimmic/PatternBase.h"
#include "Interface/PatternSpawnable.h"

UAnimNotify_ShotPattern::UAnimNotify_ShotPattern()
{
}

void UAnimNotify_ShotPattern::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ensure(IsValid(PatternClass));

	IPatternSpawnable* PatternSpawnPawn = Cast<IPatternSpawnable>(MeshComp->GetOwner());
	if (PatternSpawnPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ShotPattern Notify] SkeletalMesh owner doesn't inherit IPatternSpawnable"));
		return;
	}
	PatternSpawnPawn->SpawnPatternManager(PatternClass, FName(AttachSocketName));
}
