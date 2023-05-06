// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ShotPattern.generated.h"

/**
 * 
 */
UCLASS(Config = Game, meta = (DisplayName = "Shot Pattern"))
class EXECUTER_API UAnimNotify_ShotPattern : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_ShotPattern();
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// Spawn pattern
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	TSubclassOf<class APatternBase> PatternClass;

	// Socket or bone name to attach pattern
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	FString AttachSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	uint8 bAttachLocation : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	uint8 bAttachRotation : 1;
};
