// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_AttackCollider.h"

UAnimNotifyState_AttackCollider::UAnimNotifyState_AttackCollider()
{
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));

	CheckValue = FCheckValue();
	bOnDebug = false;
}

void UAnimNotifyState_AttackCollider::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	IAttackCheckable* AttackablePawn = Cast<IAttackCheckable>(MeshComp->GetOwner());
	if (AttackablePawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Notify Detect Target] SkeletalMesh owner doesn't inherit IAttackablePawn"));
		return;
	}

	IgnoreActorArray.Empty();
	IgnoreActorArray.Emplace(MeshComp->GetOwner());

	for (const auto SocketName : SocketNames)
	{
		if (MeshComp->DoesSocketExist(SocketName) == false)
		{
			continue;
		}

		ValidSockets.Emplace(SocketName);
	}

	CheckValue.SetSphereValue(SphereRadius);
	CheckValue.SetBoxValue(BoxWidth, BoxHeight);
	CheckValue.bDebugTrace = bOnDebug;
}

void UAnimNotifyState_AttackCollider::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckCollision(MeshComp);
}

void UAnimNotifyState_AttackCollider::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_AttackCollider::CheckCollision(USkeletalMeshComponent* MeshComp)
{
	IAttackCheckable* AttackablePawn = Cast<IAttackCheckable>(MeshComp->GetOwner());
	if (AttackablePawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Notify Detect Target] SkeletalMesh owner doesn't inherit IAttackablePawn"));
		return;
	}

	for (int32 Index = 0; Index < ValidSockets.Num(); Index++)
	{
		FHitResult TraceHit;
		bool IsCollide = AttackablePawn->CheckAttachToSocket(ValidSockets[Index], CheckType, CheckValue, ObjectTypesArray, IgnoreActorArray, TraceHit);

		if (IsCollide)
		{
			AActor* HitActor = TraceHit.GetActor();
			if (IsValid(HitActor) && IgnoreActorArray.Contains(HitActor) == false)
			{
				IgnoreActorArray.Emplace(HitActor);
			}
		}
	}
}
