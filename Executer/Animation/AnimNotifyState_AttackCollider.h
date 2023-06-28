// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Interface/AttackCheckable.h"

#include "AnimNotifyState_AttackCollider.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UAnimNotifyState_AttackCollider : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UAnimNotifyState_AttackCollider();

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	TArray<FName> SocketNames;

private:
	void CheckCollision(USkeletalMeshComponent* MeshComp);

	// Collision Check Section
private:
	UPROPERTY()
	TArray<FName> ValidSockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify", meta = (AllowPrivateAccess = "true"))
	ECheckType CheckType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify", meta = (AllowPrivateAccess = "true"))
	float SphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify", meta = (AllowPrivateAccess = "true"))
	float BoxWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify", meta = (AllowPrivateAccess = "true"))
	float BoxHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify", meta = (AllowPrivateAccess = "true"))
	uint8 bOnDebug : 1;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActorArray;

	FCheckValue CheckValue;
};
