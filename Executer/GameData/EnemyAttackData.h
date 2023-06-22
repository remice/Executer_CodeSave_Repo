
#pragma once

#include "BaseEnemyAttackData.h"
#include "EnemyAttackData.generated.h"

USTRUCT(BlueprintType)
struct FProjectileData : public FBaseEnemyAttackData
{
	GENERATED_BODY()

public:
	FProjectileData() : Speed(0), Size(1) {}

public:
	UPROPERTY(EditAnywhere, Category = "Data")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Data")
	float Size;
};

USTRUCT(BlueprintType)
struct FLasorData : public FBaseEnemyAttackData
{
	GENERATED_BODY()

public:
	FLasorData() : Size(1), HitRate(1) {}

public:
	UPROPERTY(EditAnywhere, Category = "Data")
	float Size;

	UPROPERTY(EditAnywhere, Category = "Data")
	float HitRate;
};

USTRUCT(BlueprintType)
struct FBombData : public FBaseEnemyAttackData
{
	GENERATED_BODY()

	public:
	FBombData() : Size(1) {}

public:
	UPROPERTY(EditAnywhere, Category = "Data")
	float Size;
};

USTRUCT(BlueprintType)
struct FEnemyAttackData : public FBaseEnemyAttackData
{
	GENERATED_BODY()
	
public:
	FEnemyAttackData() {}
};