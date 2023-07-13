// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "Interface/PatternSpawnable.h"
#include "Interface/AnimationTaskable.h"

#include "EnemyTuto.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AEnemyTuto : public ABaseEnemy
{
	GENERATED_BODY()
	
	
public:
	AEnemyTuto();

	virtual void BeginPlay() override;

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void OnDeath() override;

private:
	void PlayMontage(class UAnimMontage* Montage, bool IsPrimitive = true);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Data")
	TObjectPtr<class UHitMontageDataAsset> HitMontages;
};
