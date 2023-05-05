// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "Interface/PatternSpawnable.h"
#include "BossBase.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API ABossBase : public ABaseEnemy, public IPatternSpawnable
{
	GENERATED_BODY()
	
public:
	ABossBase();

public:
	virtual void SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName) override;

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;
};
