// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PatternBase.h"
#include "InstancedPattern.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AInstancedPattern : public APatternBase
{
	GENERATED_BODY()
	
public:
	AInstancedPattern();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void ExFire() override;

	virtual void SpawnBullets() override;

	UFUNCTION()
	virtual void OnCollideSomething(const FHitResult& HitResult, const FTransform& ComponentTransform);

private:
	UPROPERTY(EditAnywhere, Category = Pattern)
	float BulletDamage;

	UPROPERTY(EditAnywhere, Category = Pattern)
	float DelayBetweenSpawn;

	UPROPERTY(EditAnywhere, Category = Pattern)
	float NextDelay;

	UPROPERTY(EditAnywhere, Category = Pattern)
	float ProjectileSpeed;

	UPROPERTY(EditAnywhere, Category = Pattern)
	float ComponentSize;

	UPROPERTY(EditAnywhere, Category = Pattern)
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(EditAnywhere, Category = Collision)
	float CollisionRadius;

	UPROPERTY(EditAnywhere, Category = Collision)
	float CheckLevel1Distance;

	UPROPERTY(EditAnywhere, Category = Collision)
	float CheckLevel2Distance;

	UPROPERTY(EditAnywhere, Category = Collision)
	float CheckLevel3Distance;

	UPROPERTY(EditAnywhere, Category = Collision)
	float Level1CollisionCheckDelay;

	UPROPERTY(EditAnywhere, Category = Collision)
	float Level2CollisionCheckDelay;

	float NexLevel1CollisionCheckDelay;
	float NexLevel2CollisionCheckDelay;

	UPROPERTY()
	TObjectPtr<class UInstancedStaticMeshComponent> InstancedStaticMeshes;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypesArray;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActorArray;

	FVector CharacterLocation;

	UPROPERTY()
	TArray<int32> DestroyInstanceIds;

public:
	UFUNCTION()
	FORCEINLINE void SetDelayBetweenSpawn(const float InDelayBetweenSpawn) { DelayBetweenSpawn = InDelayBetweenSpawn; }

private:
	UFUNCTION()
	void UpdateTransformInstanceMeshes();

};
