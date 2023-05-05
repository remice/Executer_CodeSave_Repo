// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShotPattern.h"
#include "Gimmic/PatternBase.h"

UAnimNotify_ShotPattern::UAnimNotify_ShotPattern()
{
}

void UAnimNotify_ShotPattern::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ensure(IsValid(Pattern));

	UWorld* World = MeshComp->GetWorld();
	ensure(IsValid(World));

	AActor* OwnerPawn = MeshComp->GetOwner();
}
