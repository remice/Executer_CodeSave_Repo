// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ExecuterPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeadDelegate)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurHp*/)

/**
 * 
 */
UCLASS()
class EXECUTER_API AExecuterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	float GetDamaged(const float Damage);

	UFUNCTION()
	void SetupHealth(const float InMaxHealth);

	UFUNCTION()
	void SetupArmor(const float InArmor);

	UFUNCTION()
	FORCEINLINE float GetHealth() const { return CurHealth; }

// Delegate Section
public:
	FOnPlayerDeadDelegate OnPlayerDead;
	FOnHpChangedDelegate OnHpChanged;

private:
	void ChangeHealth(const float NewHp);
	
private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> Character;

	UPROPERTY()
	float CurHealth;

	UPROPERTY()
	float MaxHealth;

	UPROPERTY()
	float Armor;
};
