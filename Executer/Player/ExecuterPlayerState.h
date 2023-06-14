// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Interface/Initializable.h"
#include "ExecuterPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerDeadDelegate)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHpChangedDelegate, float, CurHp, float, MaxHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpecialChangedDelegate, float, CurSpecialGauge, float, MaxSpecialGauge);

/**
 * 
 */
UCLASS()
class EXECUTER_API AExecuterPlayerState : public APlayerState, public IInitializable
{
	GENERATED_BODY()
	
public:
	AExecuterPlayerState();

	virtual void PostInitializeComponents() override;

public:
	virtual void CallInitialize() override;

	float GetDamaged(const float Damage);
	void GetDodged(const float& DodgeAmount);
	void SetupHealth(const float InMaxHealth);
	void SetupArmor(const float InArmor);
	void SetupSpecial(const float InMaxSpecialGauge);
	
	FORCEINLINE float GetHealth() const { return CurHealth; }
	FORCEINLINE float GetSpecialGauge() const { return CurSpecialGauge; }

// Delegate Section
public:
	FOnPlayerDeadDelegate OnPlayerDead;

	UPROPERTY(BlueprintAssignable, Category = "Hp")
	FOnHpChangedDelegate OnHpChanged;

	UPROPERTY(BlueprintAssignable, Category = "SpecialGauge")
	FOnSpecialChangedDelegate OnSpecialChanged;

private:
	void ChangeHealth(const float& NewHp);
	void ChangeSpecial(const float& NewSpecialGauge);
	
private:
	UPROPERTY()
	TObjectPtr<class APlayerCharacter> Character;
	
	float CurHealth;
	float MaxHealth;
	float Armor;
	float CurSpecialGauge;
	float MaxSpecialGauge;
};
