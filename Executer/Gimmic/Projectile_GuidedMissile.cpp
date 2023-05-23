// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_GuidedMissile.h"
#include "Kismet/KismetMathLibrary.h"

AProjectile_GuidedMissile::AProjectile_GuidedMissile()
{
	PrimaryActorTick.bCanEverTick = true;

	GetProjectileMovement()->bAutoActivate = false;
	GetProjectileMovement()->Deactivate();

	Velocity = FVector::ZeroVector;
	TargetLocation = FVector::ZeroVector;
	bOnHoming = false;
	HomingSetting = FHomingSetting();
}

void AProjectile_GuidedMissile::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GetGameInstance()) == false)
	{
		return;
	}

	AActor* Target = GetGameInstance()->GetFirstLocalPlayerController()->GetPawn();
	if (IsValid(Target) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Projectile_DirectToPlayer] Target is null"));
		return;
	}

	TargetLocation = Target->GetActorLocation();
	MiddlePoint = CalcMidPoint();
	CurvedPoint = CalcCurvePoint(MiddlePoint);
	bOnHoming = true;
}

void AProjectile_GuidedMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnHoming == false)
	{
		SetActorLocation(GetActorLocation() + Velocity.GetSafeNormal() * HomingSetting.Speed * DeltaTime);
		return;
	}

	if (DeltaTime < 0.001f)
	{
		return;
	}

	Velocity = TargetLocation - GetActorLocation();

	if (Velocity.Size() < 100)
	{
		bOnHoming = false;
		return;
	}

	CurvedPoint = UKismetMathLibrary::VInterpTo_Constant(CurvedPoint, TargetLocation, DeltaTime, HomingSetting.Speed);
	FVector NewLocation = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), CurvedPoint, DeltaTime, HomingSetting.Speed);
	SetActorLocation(NewLocation, true);
}

FVector AProjectile_GuidedMissile::CalcMidPoint()
{
	FVector OriginError = TargetLocation - GetActorLocation();
	float RandDistance = FMath::RandRange(HomingSetting.MinCurvePointDistance, HomingSetting.MaxCurvePointDistance);
	FVector RandDisError = OriginError * RandDistance;
	FVector Result = RandDisError + GetActorLocation();

	return Result;
}

FVector AProjectile_GuidedMissile::CalcCurvePoint(FVector MidPoint)
{
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(MidPoint, TargetLocation);
	float RandRadius = FMath::RandRange(HomingSetting.MinCurveRadius, HomingSetting.MaxCurveRadius);
	FVector RotBreakToX;
	FVector RotBreakToY;
	FVector RotBreakToZ;
	UKismetMathLibrary::BreakRotIntoAxes(TargetRotator, RotBreakToX, RotBreakToY, RotBreakToZ);

	FVector ScaledRotBreakToY = RotBreakToY * RandRadius;
	float RandAngle = FMath::RandRange(HomingSetting.MinAngle, HomingSetting.MaxAngle);
	FVector RotatedVector = ScaledRotBreakToY.RotateAngleAxis(RandAngle, RotBreakToX);
	FVector Result = RotatedVector + MidPoint;

	return Result;
}
