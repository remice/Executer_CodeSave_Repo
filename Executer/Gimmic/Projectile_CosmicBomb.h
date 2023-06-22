// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/Projectile.h"
#include "Projectile_CosmicBomb.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AProjectile_CosmicBomb : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_CosmicBomb();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void Explosion();

private:
	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveVector> MoveCurve;

	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCurveFloat> ScaleCurve;

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackSphereRadius;

	FVector SavePos;

	double MaxMoveCurveTime = 0;
	double MaxScaleCurveTime = 0;
	float CurTime = 0;
};
