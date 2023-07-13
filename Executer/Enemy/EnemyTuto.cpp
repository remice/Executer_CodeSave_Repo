// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyTuto.h"

#include "Animation/HitMontageDataAsset.h"

AEnemyTuto::AEnemyTuto()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Mesh->SetupAttachment(GetCapsuleComponent());
}

void AEnemyTuto::BeginPlay()
{
	Super::BeginPlay();

	CallInitialize();
}

float AEnemyTuto::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageCauser)
	{
		FVector DamageDirection = DamageCauser->GetActorLocation() - GetActorLocation();
		DamageDirection.Normalize();

		float DamageDegree = FMath::RadiansToDegrees(FMath::Atan2(DamageDirection.Y, DamageDirection.X));

		if (DamageDegree > -45 && DamageDegree < 45)
		{
			PlayMontage(HitMontages->FrontHitMontage);
		}
		else if (DamageDegree < -135 || DamageDegree > 135)
		{
			PlayMontage(HitMontages->BackHitMontage);
		}
		else
		{
			double DotRightVector = DamageDirection.Dot(GetActorRightVector());
			if (DotRightVector > 0)
			{
				PlayMontage(HitMontages->RightHitMontage);
			}
			else
			{
				PlayMontage(HitMontages->LeftHitMontage);
			}
		}
	}

	return ResultDamage;
}

void AEnemyTuto::OnDeath()
{
	Super::OnDeath();

	if (DeathMontage == nullptr) return;

	UAnimInstance* AI = Mesh->GetAnimInstance();
	if (IsValid(AI) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyTuto] AnimInstance are not exist!!"));
		return;
	}

	AI->Montage_Play(DeathMontage);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]() {
			if (bIsDead) return;

			bIsDead = true;

			DetachFromControllerPendingDestroy();

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

			Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
			Mesh->SetAllBodiesSimulatePhysics(true);
			Mesh->SetSimulatePhysics(true);
			Mesh->WakeAllRigidBodies();
			Mesh->bBlendPhysics = true;
		}
	), 0.1f, false, AfterDeathTime);
}

void AEnemyTuto::PlayMontage(UAnimMontage* Montage, bool IsPrimitive)
{
	if (IsValid(Mesh->GetAnimInstance()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyTuto] AnimInstance are not exist!!"));
		return;
	}

	if (IsPrimitive && Mesh->GetAnimInstance()->Montage_IsPlaying(nullptr)) return;

	Mesh->GetAnimInstance()->Montage_Play(Montage);
}
