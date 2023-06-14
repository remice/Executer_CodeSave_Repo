// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuterPlayerState.h"

AExecuterPlayerState::AExecuterPlayerState()
{
	CurHealth = -1.f;
	MaxHealth = -1.f;
	Armor = 0.f;
	CurSpecialGauge = -1.f;
	MaxSpecialGauge = -1.f;
}

void AExecuterPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AExecuterPlayerState::CallInitialize()
{
	SetupSpecial(50);
	SetupHealth(1000);
	SetupArmor(5);
}

float AExecuterPlayerState::GetDamaged(const float Damage)
{
	const float DamageModulator = 100 / (Armor + 100);

	float FinalDamage = Damage - Armor;
	FinalDamage *= DamageModulator;
	FinalDamage = FinalDamage < 1 ? 1 : FinalDamage;

	ChangeHealth(CurHealth - FinalDamage);

	return FinalDamage;
}

void AExecuterPlayerState::GetDodged(const float& DodgeLevel)
{
	float FinalDodgeValue = FMath::Clamp(CurSpecialGauge + DodgeLevel, 0, MaxSpecialGauge);

	ChangeSpecial(FinalDodgeValue);
}

void AExecuterPlayerState::SetupHealth(const float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	ChangeHealth(MaxHealth);
}

void AExecuterPlayerState::SetupArmor(const float InArmor)
{
	Armor = InArmor;
}

void AExecuterPlayerState::SetupSpecial(const float InMaxSpecialGauge)
{
	MaxSpecialGauge = InMaxSpecialGauge;
	ChangeSpecial(0);
}

void AExecuterPlayerState::ChangeHealth(const float& NewHp)
{
	const float ActualHealth = FMath::Clamp(NewHp, 0, MaxHealth);

	if (CurHealth == ActualHealth)
	{
		return;
	}

	CurHealth = ActualHealth;

	if (CurHealth < KINDA_SMALL_NUMBER)
	{
		OnPlayerDead.Broadcast();
	}

	OnHpChanged.Broadcast(CurHealth, MaxHealth);
}

void AExecuterPlayerState::ChangeSpecial(const float& NewSpecialGauge)
{
	const float ActualGauge = FMath::Clamp(NewSpecialGauge, 0, MaxSpecialGauge);

	if (CurSpecialGauge == ActualGauge)
	{
		return;
	}

	CurSpecialGauge = ActualGauge;
	OnSpecialChanged.Broadcast(CurSpecialGauge, MaxSpecialGauge);
}