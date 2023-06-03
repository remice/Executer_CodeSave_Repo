// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Pattern_RepeatOneShot.h"
#include "Interface/IRotatableObject.h"
#include "Components/ArrowComponent.h"
#include "AttackBase.h"

APattern_RepeatOneShot::APattern_RepeatOneShot()
{
}

void APattern_RepeatOneShot::ExFire()
{
	APatternBase::ExFire();

	if (CurPatternTime < CurShotDelay)
	{
		return;
	}

	CurShotDelay += ShotDelay;

	SpawnBullets();
}

void APattern_RepeatOneShot::SpawnBullets()
{
	APatternBase::SpawnBullets();

	if (IsValid(SpawnBulletClass) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pattern_RepeatOneShot/SpawnBulletClass is invalid, check property"));
		return;
	}

	for (const TObjectPtr<UArrowComponent> FirePoint : FirePointArray)
	{
		IIRotatableObject* RotatableObject = Cast<IIRotatableObject>(FirePoint);

		if (RotatableObject)
		{
			RotatableObject->RelativeRotate(CurPatternTime);
		}

		FVector3d SpawnLoc = FirePoint->GetComponentLocation();
		FRotator3d SpawnRot = FirePoint->GetComponentRotation();

		FActorSpawnParameters SpawnParameters;
		AActor* SpawnObject = GetWorld()->SpawnActor(SpawnBulletClass, &SpawnLoc, &SpawnRot, SpawnParameters);
		AAttackBase* Projectile = Cast<AAttackBase>(SpawnObject);
		if (Projectile == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pattern_RepeatOneShot/Cast to Projectil was failed."));
			continue;
		}
		Projectile->SetId(PatternId);
		PatternId++;
	}
}
