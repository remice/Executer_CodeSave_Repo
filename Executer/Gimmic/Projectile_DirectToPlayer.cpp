// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Projectile_DirectToPlayer.h"

AProjectile_DirectToPlayer::AProjectile_DirectToPlayer()
{
}

void AProjectile_DirectToPlayer::BeginPlay()
{
	FRotator LookAtRotator;
	SetActorRotation(LookAtRotator);
}
