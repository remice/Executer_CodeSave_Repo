// Fill out your copyright notice in the Description page of Project Settings.


#include "EXGameSingleton.h"

#include "DataAssetRef.h"
#include "EnemyAttackData.h"

UEXGameSingleton::UEXGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ProjectileTableRef(PROJECTILE_REF);
	static ConstructorHelpers::FObjectFinder<UDataTable> LasorTableRef(LASOR_REF);

	check(ProjectileTableRef.Succeeded());
	check(LasorTableRef.Succeeded());

	const UDataTable* LasorDataTable = LasorTableRef.Object;
	check(LasorDataTable->GetRowMap().Num() > 0);
	const UDataTable* ProjectileDataTable = ProjectileTableRef.Object;
	check(ProjectileDataTable->GetRowMap().Num() > 0);

	TArray<uint8*> ValueArray;
	LasorDataTable->GetRowMap().GenerateValueArray(ValueArray);
	FEnemyAttackDataStruct LasorStruct = FEnemyAttackDataStruct(ValueArray);

	ProjectileDataTable->GetRowMap().GenerateValueArray(ValueArray);
	FEnemyAttackDataStruct ProjectileStruct = FEnemyAttackDataStruct(ValueArray);
	
	EnemyAttackDataMap.Emplace(EEnemyAttackData::Lasor, LasorStruct);
	EnemyAttackDataMap.Emplace(EEnemyAttackData::Projectile, ProjectileStruct);
}

UEXGameSingleton& UEXGameSingleton::Get()
{
	UEXGameSingleton* SingletonObject = CastChecked<UEXGameSingleton>(GEngine->GameSingleton.Get());
	return *SingletonObject;
}
