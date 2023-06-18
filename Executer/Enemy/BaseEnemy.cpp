// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "Game/MyGameInstance.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->SetCapsuleRadius(44.f);
	Collider->SetCapsuleHalfHeight(88.f);

	// Set roation rule
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ABaseEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->SetMapBoss(this);
	}

	HP = MaxHP;
}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	float NewHP = FMath::Clamp(HP - DamageAmount, 0, MaxHP);
	float ActualDamage = FMath::Clamp(HP - NewHP, 0, MaxHP);
	HP = NewHP;
	OnHpChanged.Broadcast(HP, MaxHP);

	if (HP <= 0)
	{
		Destroy();
	}

	return ActualDamage;
}

void ABaseEnemy::CallInitialize()
{
	SetupHp(10000);
}

void ABaseEnemy::SetupHp(float InMaxHp)
{
	MaxHP = InMaxHp;
	HP = MaxHP;
	OnHpChanged.Broadcast(HP, MaxHP);
}
