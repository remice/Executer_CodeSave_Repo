// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState_DetectTarget.h"
#include "Interface/AttackCheckable.h"

UAnimNotifyState_DetectTarget::UAnimNotifyState_DetectTarget()
{
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
}

void UAnimNotifyState_DetectTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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

	SaveSocketLocations(MeshComp);
}

void UAnimNotifyState_DetectTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckCollision(MeshComp);
	SaveSocketLocations(MeshComp);
}

void UAnimNotifyState_DetectTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}

void UAnimNotifyState_DetectTarget::SaveSocketLocations(USkeletalMeshComponent* MeshComp)
{
	IAttackCheckable* AttackablePawn = Cast<IAttackCheckable>(MeshComp->GetOwner());
	if (AttackablePawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Notify Detect Target] SkeletalMesh owner doesn't inherit IAttackablePawn"));
		return;
	}

	if (ValidSockets.Num() != PreFrameLocations.Num())
	{
		// Init PreFrameLocations
		PreFrameLocations.Empty();

		for (const auto ValidSocketName : ValidSockets)
		{
			FVector SocketLocation = AttackablePawn->GetLocationToSocket(ValidSocketName);
			PreFrameLocations.Emplace(SocketLocation);
		}
		return;
	}

	for (int32 Index = 0; Index < ValidSockets.Num(); Index++)
	{
		FVector SocketLocation = AttackablePawn->GetLocationToSocket(ValidSockets[Index]);
		PreFrameLocations[Index] = SocketLocation;
	}
}

void UAnimNotifyState_DetectTarget::CheckCollision(USkeletalMeshComponent* MeshComp)
{
	IAttackCheckable* AttackablePawn = Cast<IAttackCheckable>(MeshComp->GetOwner());
	if (AttackablePawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Notify Detect Target] SkeletalMesh owner doesn't inherit IAttackablePawn"));
		return;
	}

	if (ValidSockets.Num() != PreFrameLocations.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Notify Detect Target] Valid Sockets, Pre Frame Locations size are different!!"));
		return;
	}

	for (int32 Index = 0; Index < ValidSockets.Num(); Index++)
	{
		FHitResult TraceHit;
		bool IsCollide = AttackablePawn->CheckAttachToSocket(ValidSockets[Index], PreFrameLocations[Index], ObjectTypesArray, IgnoreActorArray, TraceHit);

		if (IsCollide)
		{
			AActor* HitActor = TraceHit.GetActor();
			if (IsValid(HitActor) && IgnoreActorArray.Contains(HitActor) == false)
			{
				UE_LOG(LogTemp, Log, TEXT("Hit Actor!!"));
				IgnoreActorArray.Emplace(HitActor);
			}
		}
	}
}
