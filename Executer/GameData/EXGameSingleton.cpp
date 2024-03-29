// Fill out your copyright notice in the Description page of Project Settings.


#include "EXGameSingleton.h"

#include "DataAssetRef.h"
#include "EnemyAttackData.h"

UEXGameSingleton::UEXGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileTableRef(PROJECTILE_REF);
	static ConstructorHelpers::FObjectFinder<UDataTable> LasorTableRef(LASOR_REF);
	static ConstructorHelpers::FObjectFinder<UDataTable> BombTableRef(BOMB_REF);

	check(ProjectileTableRef.Succeeded());
	check(LasorTableRef.Succeeded());
	check(BombTableRef.Succeeded());

	AddAttackDataToMap(ProjectileTableRef.Object, EAttackData::Projectile);
	AddAttackDataToMap(LasorTableRef.Object, EAttackData::Lasor);
	AddAttackDataToMap(BombTableRef.Object, EAttackData::Bomb);
}

UEXGameSingleton& UEXGameSingleton::Get()
{
	UEXGameSingleton* SingletonObject = CastChecked<UEXGameSingleton>(GEngine->GameSingleton.Get());
	return *SingletonObject;
}

void UEXGameSingleton::AddAttackDataToMap(const UDataTable* InDataTable, EAttackData DataType)
{
	check(InDataTable->GetRowMap().Num() > 0);

	TArray<uint8*> ValueArray;
	InDataTable->GetRowMap().GenerateValueArray(ValueArray);
	FEnemyAttackDataStruct DataStruct = FEnemyAttackDataStruct(ValueArray);

	EnemyAttackDataMap.Emplace(DataType, DataStruct);
}
