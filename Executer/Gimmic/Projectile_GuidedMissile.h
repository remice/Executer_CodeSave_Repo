// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/Projectile.h"
#include "Projectile_GuidedMissile.generated.h"


USTRUCT()
struct FHomingSetting 
{
	GENERATED_BODY()
	
	FHomingSetting()
	{
		Speed = 800.f;
		MinCurveRadius = 40.f;
		MaxCurveRadius = 400.f;
		MinCurvePointDistance = 0.2f;
		MaxCurvePointDistance = 0.8f;
		MinAngle = 0.f;
		MaxAngle = 360.f;
	}

	UPROPERTY(EditAnywhere, Category = "Homing")
	float Speed;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MinCurveRadius;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MaxCurveRadius;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MinCurvePointDistance;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MaxCurvePointDistance;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MinAngle;
	UPROPERTY(EditAnywhere, Category = "Homing")
	float MaxAngle;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API AProjectile_GuidedMissile : public AProjectile
{
	GENERATED_BODY()
	
public:
	AProjectile_GuidedMissile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	// return mid point of random radius circle from target to this
	FVector CalcMidPoint();
	// return curve point target-midpoint-this to random angle
	FVector CalcCurvePoint(FVector MidPoint);

protected:
	UPROPERTY(VisibleAnywhere, Category="Homing")
	FVector Velocity;
	FVector TargetLocation;
	FVector MiddlePoint;
	FVector CurvedPoint;

	uint8 bOnHoming : 1;
	UPROPERTY(EditAnywhere, Category="Homing")
	FHomingSetting HomingSetting;
};