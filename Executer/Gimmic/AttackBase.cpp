// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/AttackBase.h"

#include "GameData/EnemyAttackData.h"

// Sets default values
AAttackBase::AAttackBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rAttackLevel = 0;
	rAttackType = EAttackData::Default;
	rId = FMath::Rand();
}

void AAttackBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAttackBase::DestroySelf()
{
	Destroy();
	return;
}

float AAttackBase::GetDamage() const
{
	FEnemyAttackDataStruct AttackData = UEXGameSingleton::Get().EnemyAttackDataMap[rAttackType];
	const FBaseEnemyAttackData* EnemyAttackData = AttackData.GetValidData<FBaseEnemyAttackData>(rAttackLevel);
	return EnemyAttackData->Damage;
}

