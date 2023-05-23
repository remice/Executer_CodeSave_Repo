// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Projectile_DirectToPlayer.h"
#include "Kismet/KismetMathLibrary.h"

AProjectile_DirectToPlayer::AProjectile_DirectToPlayer()
{
}

void AProjectile_DirectToPlayer::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (IsValid(GetGameInstance()) == false)
	{
		return;
	}

	AActor* Target = GetGameInstance()->GetFirstLocalPlayerController()->GetPawn();
	if (IsValid(Target) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Projectile_DirectToPlayer] Target is null"));
		return;
	}

	FRotator Direction = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	SetActorRotation(Direction);
}
