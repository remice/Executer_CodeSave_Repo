// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/EXGameSingleton.h"

#include "AttackBase.generated.h"

UCLASS()
class EXECUTER_API AAttackBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackBase();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int32 AttackLevel;

	UPROPERTY()
	int32 Id;

	EEnemyAttackData AttackType;

public:
	FORCEINLINE float GetAttackLevel() const { return AttackLevel; }
	FORCEINLINE void SetId(const int32& InId) { Id = InId; }
	float GetDamage() const;
};
