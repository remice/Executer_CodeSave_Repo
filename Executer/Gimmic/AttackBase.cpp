// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/AttackBase.h"

// Sets default values
AAttackBase::AAttackBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AttackLevel = 0;
	Damage = 50.f;
	Id = FMath::Rand();
}

