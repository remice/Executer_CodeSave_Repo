// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuterPlayerState.h"
#include "Character/PlayerCharacter.h"

void AExecuterPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetupHealth(1000);
	SetupArmor(5);
}

void AExecuterPlayerState::GetDamaged(const float Damage)
{
	if (IsValid(Character) == false)
	{
		Character = Cast<APlayerCharacter>(GetPawn());

		if (IsValid(Character) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player state doesn't have player!!"));
			return;
		}
	}
	Character->DisableDodge(1.f);

	float DamageModulator = 100 / (Armor + 100);
	float FinalDamage = Damage - Armor;
	FinalDamage *= DamageModulator;
	FinalDamage = FinalDamage < 1 ? 1 : FinalDamage;

	CurHealth -= FinalDamage;

	if (CurHealth <= 0)
	{
		// player death
	}
}

void AExecuterPlayerState::SetupHealth(const float InMaxHealth)
{
	MaxHealth = InMaxHealth;
	CurHealth = MaxHealth;
}

void AExecuterPlayerState::SetupArmor(const float InArmor)
{
	Armor = InArmor;
}