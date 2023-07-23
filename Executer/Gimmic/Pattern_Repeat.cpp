// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Pattern_Repeat.h"
#include "Interface/IRotatableObject.h"
#include "Components/ArrowComponent.h"
#include "BaseAttack.h"

APattern_Repeat::APattern_Repeat()
{
	NextDelay = 0.5f;
	CurDelay = 0.f;
}

void APattern_Repeat::ExFire()
{
	Super::ExFire();

	if (CurPatternTime < CurDelay)
	{
		return;
	}

	CurDelay += NextDelay;
	SpawnBullets();
}

void APattern_Repeat::SpawnBullets()
{
	APatternBase::SpawnBullets();

	if (IsValid(SpawnBulletClass) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Pattern_Repeat] SpawnBulletClass is invalid, check property"));
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
		ABaseAttack* AttackCauser = Cast<ABaseAttack>(SpawnObject);
		if (AttackCauser == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[Pattern_Repeat] Cast to BaseAttack was failed."));
			continue;
		}
		AttackCauser->SetId(PatternId);
		BulletArray.Emplace(AttackCauser);
		PatternId++;
	}
}
