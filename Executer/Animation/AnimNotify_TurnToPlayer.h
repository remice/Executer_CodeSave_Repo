// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_TurnToPlayer.generated.h"

/**
 * 
 */
UCLASS(Config = Game, meta = (DisplayName = "Turn To Player"))
class EXECUTER_API UAnimNotify_TurnToPlayer : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_TurnToPlayer();
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Setting")
	float InterpSpeed;
};
