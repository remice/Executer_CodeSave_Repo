// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SpawnActor.h"

UAnimNotify_SpawnActor::UAnimNotify_SpawnActor()
{
	AdjustSpawnPos = FVector::ZeroVector;
}

void UAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* OwningPawn = MeshComp->GetOwner();
	if (IsValid(OwningPawn) == false)
	{
		return;
	}

	FTransform SpawnTransform = FTransform::Identity;
	if (MeshComp->DoesSocketExist(PointSocketName))
	{
		SpawnTransform = MeshComp->GetSocketTransform(PointSocketName);
	}
	SpawnTransform.SetLocation(SpawnTransform.GetLocation() + AdjustSpawnPos);

	OwningPawn->GetWorld()->SpawnActor(SpawnActorClass, &SpawnTransform);
}
