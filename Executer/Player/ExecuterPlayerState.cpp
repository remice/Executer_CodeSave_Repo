// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuterPlayerState.h"

void AExecuterPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AExecuterPlayerState::Initialize()
{
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

void AExecuterPlayerState::SetupHealth(const float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	ChangeHealth(MaxHealth);
}

void AExecuterPlayerState::SetupArmor(const float InArmor)
{
	Armor = InArmor;
}

void AExecuterPlayerState::ChangeHealth(const float NewHp)
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
