// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Projectile_Smart.h"

#include "Kismet/KismetSystemLibrary.h"

AProjectile_Smart::AProjectile_Smart()
{
	PrimaryActorTick.bCanEverTick = true;

	MaxZVelocity = 50.f;
	MinZVelocity = -50.f;
	bOnDebugLine = false;
}

void AProjectile_Smart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

bool AProjectile_Smart::CheckCeiling()
{
	
	check(GetWorld());
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActors;
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	FHitResult HitResult;
	bool bIsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorUpVector() * CheckCeilLineLength,
		ObjectTypesArray, false, IgnoreActors, bOnDebugLine ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None, HitResult, true);
	if (bIsCollide)
	{
		return true;
	}
	return false;
	
}
