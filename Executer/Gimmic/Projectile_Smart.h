// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/Projectile.h"
#include "Projectile_Smart.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AProjectile_Smart : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_Smart();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual bool ScaleVelocity();
	virtual bool CheckCeiling();
	
private:
	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float MaxZVelocity;

	UPROPERTY(EditAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true", ClampMax = "0"))
	float MinZVelocity;

	UPROPERTY(EditAnywhere, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	float CheckCeilLineLength;

	UPROPERTY(EditAnywhere, Category = "Trace", meta = (AllowPrivateAccess = "true"))
	uint8 bOnDebugLine : 1;
};
