// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Projectile_Smart.h"

#include "Kismet/KismetSystemLibrary.h"

AProjectile_Smart::AProjectile_Smart()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxZVelocity = 50.f;
	MinZVelocity = -50.f;
	UpGravityScaler = 1.5f;
	DownGravityScaler = 1.f;
	CheckCeilLineLength = 100.f;
	CheckFloorLineLength = 200.f;
	GravityInterpScale = 10.f;
	bOnDebugLine = false;

	GetBody()->SetEnableGravity(true);
}

void AProjectile_Smart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmartMove(DeltaTime);
	ScaleVelocity();
}

bool AProjectile_Smart::ScaleVelocity()
{
	if (GetProjectileMovement() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AProjectile_Smart] ProjectileMovement has not exist!!"));
		return false;
	}

	FVector PreVelocity = GetProjectileMovement()->Velocity;
	float ClampVelocityZ = FMath::Clamp(PreVelocity.Z, MinZVelocity, MaxZVelocity);
	GetProjectileMovement()->Velocity = FVector(PreVelocity.X, PreVelocity.Y, ClampVelocityZ);
	return true;
}

bool AProjectile_Smart::CheckGeometry(bool bIsUp)
{
	check(GetWorld());
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActors;
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FHitResult HitResult;

	FVector EndPoint = bIsUp ? 
			GetActorLocation() + FVector(0, 0, 1) * CheckCeilLineLength 
		:	GetActorLocation() + FVector(0, 0, -1) * CheckFloorLineLength;
	bool bIsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), EndPoint,
		ObjectTypesArray, false, IgnoreActors, bOnDebugLine ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, HitResult, true);
	if (bIsCollide)
	{
		return true;
	}
	return false;
}

void AProjectile_Smart::SmartMove(float DeltaTime)
{
	if (CheckGeometry(true))
	{
		ScaleGravity(DownGravityScaler, DeltaTime);
	}
	else if (CheckGeometry(false))
	{
		ScaleGravity(-UpGravityScaler, DeltaTime);
	}
	else
	{
		ScaleGravity(DownGravityScaler, DeltaTime);
	}
}

void AProjectile_Smart::ScaleGravity(float ScaleAmount, float DeltaTime)
{
	float CurGravityScale = GetProjectileMovement()->ProjectileGravityScale;
	CurGravityScale += ScaleAmount * GravityInterpScale * DeltaTime;

	CurGravityScale = FMath::Clamp(CurGravityScale, -UpGravityScaler, DownGravityScaler);
	GetProjectileMovement()->ProjectileGravityScale = CurGravityScale;

	bool bIsUpMoveByGravity = CurGravityScale <= 0 && GetProjectileMovement()->Velocity.Z >= 0;
	if (bIsUpMoveByGravity)
	{
		GetProjectileMovement()->Velocity.Z = 0;
		GetProjectileMovement()->ProjectileGravityScale = 0;
	}
}