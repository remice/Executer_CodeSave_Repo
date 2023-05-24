// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackBase.generated.h"

UCLASS()
class EXECUTER_API AAttackBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackBase();

protected:
	UPROPERTY()
	float Damage;

	UPROPERTY()
	int32 Id;

public:
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE void SetId(const int32& InId) { Id = InId; }
};
