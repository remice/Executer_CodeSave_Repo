// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BossBase.h"
#include "EnemyTuto_Range.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AEnemyTuto_Range : public ABossBase
{
	GENERATED_BODY()
	
public:
	AEnemyTuto_Range();
	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnStun() override;
	virtual void EndStun() override;

protected:
	virtual void OnDeath() override;
	void PlayMontage(class UAnimMontage* Montage, bool IsPrimitive = true);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UHitMontageDataAsset> HitMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStunMontageDataAsset> StunMontageData;
};
