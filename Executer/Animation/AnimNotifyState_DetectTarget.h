// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_DetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UAnimNotifyState_DetectTarget : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_DetectTarget();

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	TArray<FName> SocketNames;

private:
	void SaveSocketLocations(USkeletalMeshComponent* MeshComp);

	void CheckCollision(USkeletalMeshComponent* MeshComp);

// Collision Check Section
private:
	UPROPERTY()
	TArray<FName> ValidSockets;

	UPROPERTY()
	TArray<FVector> PreFrameLocations;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActorArray;
};
