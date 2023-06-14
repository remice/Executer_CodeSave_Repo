
#pragma once

#include "Engine/DataTable.h"
#include "BaseEnemyAttackData.generated.h"

USTRUCT(BlueprintType)
struct FBaseEnemyAttackData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FBaseEnemyAttackData() : Damage(0) {}

public:
	UPROPERTY(EditAnywhere, Category="Data")
	float Damage;
};