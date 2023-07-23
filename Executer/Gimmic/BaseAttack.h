// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/EXGameSingleton.h"

#include "BaseAttack.generated.h"

UCLASS()
class EXECUTER_API ABaseAttack : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAttack();

	virtual void BeginPlay() override;
	virtual void DestroySelf();

protected:
	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int32 rAttackLevel;

	UPROPERTY()
	int32 rId;

	UPROPERTY(EditAnywhere, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	EAttackData rAttackType;

public:
	FORCEINLINE float GetAttackLevel() const { return rAttackLevel; }
	FORCEINLINE void SetId(const int32& InId) { rId = InId; }
	float GetDamage() const;

};
