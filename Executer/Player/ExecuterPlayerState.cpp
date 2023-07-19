// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuterPlayerState.h"
#include "Interface/ExecuterGIInterface.h"

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
	IExecuterGIInterface* EGI = Cast<IExecuterGIInterface>(GetGameInstance());
	ensure(EGI);

	SetupHealth(1000, EGI->GetSaveStat().Hp);
	SetupHealth(1000, 1000);
	SetupArmor(0);
	SetupSpecial(50, 0);
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

void AExecuterPlayerState::SetupHealth(const float InMaxHealth, float InCurHealth)
{
	MaxHealth = InMaxHealth;
	if (InCurHealth == 0) InCurHealth = MaxHealth;
	ChangeHealth(InCurHealth);
}

void AExecuterPlayerState::SetupArmor(const float InArmor)
{
	Armor = InArmor;
}

void AExecuterPlayerState::SetupSpecial(const float InMaxSpecialGauge, float InCurSpecialGauge)
{
	MaxSpecialGauge = InMaxSpecialGauge;
	ChangeSpecial(InCurSpecialGauge);
}

void AExecuterPlayerState::SpecialAttack()
{
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