// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyTuto_Range.h"

#include "Animation/HitMontageDataAsset.h"
#include "Animation/StunMontageDataAsset.h"
#include "AI/RangerAIController.h"
#include "TutoAnimInstance.h"

AEnemyTuto_Range::AEnemyTuto_Range()
{
	GetCapsuleComponent()->SetSimulatePhysics(false);
}

float AEnemyTuto_Range::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AEnemyTuto_Range::OnStun()
{
	Super::OnStun();

	if (StunMontageData && StunMontageData->StunMontage)
	{
		PlayMontage(StunMontageData->StunMontage, false);
	}
}

void AEnemyTuto_Range::EndStun()
{
	Super::EndStun();

	if (Mesh->GetAnimInstance()->Montage_IsPlaying(StunMontageData->StunMontage))
	{
		Mesh->GetAnimInstance()->Montage_JumpToSection(*StunMontageData->EndSectionName, StunMontageData->StunMontage);
	}

	ARangerAIController* AIController = Cast<ARangerAIController>(GetController());
	if (AIController)
	{
		AIController->RestartAI();
	}
}

void AEnemyTuto_Range::OnDeath()
{
	Super::OnDeath();

	if (DeathMontage == nullptr) return;

	UTutoAnimInstance* AI = Cast<UTutoAnimInstance>(Mesh->GetAnimInstance());
	if (IsValid(AI) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyTuto] AnimInstance are not exist!!"));
		return;
	}
	 
	if (AI->bOnDead) return;

	AI->Montage_Stop(0.f);
	AI->Montage_Play(DeathMontage);
	AI->bOnDead = true;

	DetachFromControllerPendingDestroy();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		[&]() {
			if (bIsDead) return;

			bIsDead = true;

			Mesh->SetAllBodiesSimulatePhysics(true);
			Mesh->SetSimulatePhysics(true);
			Mesh->WakeAllRigidBodies();
			Mesh->bBlendPhysics = true;
		}
	), 0.1f, false, AfterDeathTime);
}

void AEnemyTuto_Range::PlayMontage(UAnimMontage* Montage, bool IsPrimitive)
{
	if (IsValid(Mesh->GetAnimInstance()) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyTuto_Range] AnimInstance are not exist!!"));
		return;
	}

	if (IsPrimitive && Mesh->GetAnimInstance()->Montage_IsPlaying(nullptr)) return;

	Mesh->GetAnimInstance()->Montage_Play(Montage);
}
