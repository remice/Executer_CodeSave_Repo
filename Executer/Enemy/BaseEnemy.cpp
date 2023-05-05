// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Game/MyGameInstance.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Capsule Component
	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	SetRootComponent(Collider);
	Collider->SetCapsuleSize(44.f, 88.f);

	// Set roation rule
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	MaxHP = 10000;
	HP = 10000;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->SetMapBoss(this);
	}
	
	HP = MaxHP;
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float NewHP = FMath::Clamp(HP - DamageAmount, 0, MaxHP);
	float ActualDamage = FMath::Clamp(HP - NewHP, 0, MaxHP);
	HP = NewHP;
	OnHpChanged.Broadcast(HP);

	if (HP <= 0)
	{
		Destroy();
	}

	return ActualDamage;
}