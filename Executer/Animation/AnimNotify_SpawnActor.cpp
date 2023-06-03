// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SpawnActor.h"
#include "Kismet/KismetSystemLibrary.h"

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

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypeArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	TArray<AActor*> IgnoreActorArray;

	FHitResult HitResult;
	bool IsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(OwningPawn->GetWorld(), SpawnTransform.GetLocation(),
		SpawnTransform.GetLocation() + FVector(0, 0, -500), ObjectTypeArray, false, IgnoreActorArray, EDrawDebugTrace::None, HitResult, true);

	if (IsCollide)
	{
		SpawnTransform.SetLocation(HitResult.ImpactPoint);
		OwningPawn->GetWorld()->SpawnActor(SpawnActorClass, &SpawnTransform);
	}
}
