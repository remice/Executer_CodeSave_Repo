// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEnemyAttackData.h"

#include "EXGameSingleton.generated.h"

UENUM()
enum class EEnemyAttackData : uint8
{
	Default = 0,
	Projectile,
	Lasor,
	Bomb,
	EAD_Max,
};

USTRUCT()
struct FEnemyAttackDataStruct 
{
	GENERATED_BODY()
	
public:
	FEnemyAttackDataStruct() : DataArray(TArray<uint8*>()) {}
	FEnemyAttackDataStruct(TArray<uint8*> InDataArray) : DataArray(InDataArray) {}

public:
	template <typename T>
	const T* GetValidData(int32 Index)
	{
		if (DataArray.IsEmpty()) return nullptr;

		int32 ValidIndex = FMath::Clamp(Index, 0, DataArray.Num());
		T* CastedData = (T*)DataArray[ValidIndex];

		return CastedData;
	}

private:
	TArray<uint8*> DataArray;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API UEXGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UEXGameSingleton();

	static UEXGameSingleton& Get();

public:
	UPROPERTY()
	TMap<EEnemyAttackData, FEnemyAttackDataStruct> EnemyAttackDataMap;

private:
	void AddAttackDataToMap(const UDataTable* InDataTable, EEnemyAttackData DataType);
};
