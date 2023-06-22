// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FirstBoss.h"

#include "AI/FirstBossAIController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AITaskManager.h"
#include "Animation/HitMontageDataAsset.h"

#define PATH_ANIMINSTANCE_C TEXT("/Game/Character/Enemy/ABP_Gideon.ABP_Gideon_c")
#define PATH_SKELETALMESH TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Skins/Inquisitor/Meshes/Gideon_Inquisitor.Gideon_Inquisitor'")
#define PATH_DAHITMONTAGE TEXT("/Script/Executer.HitMontageDataAsset'/Game/DataAssets/DA_GideonHitMontage.DA_GideonHitMontage'")

AFirstBoss::AFirstBoss()
{
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(PATH_ANIMINSTANCE_C);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(PATH_SKELETALMESH);
	ConstructorHelpers::FObjectFinder<UHitMontageDataAsset> DAHitMontageRef(PATH_DAHITMONTAGE);

	ensure(AnimInstanceRef.Succeeded());
	ensure(SkeletalMeshRef.Succeeded());
	ensure(DAHitMontageRef.Succeeded());

	GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	GetMesh()->bWaitForParallelClothTask = true;

	HitMontages = DAHitMontageRef.Object;

	AIControllerClass = AFirstBossAIController::StaticClass();

	AITaskManager = CreateDefaultSubobject<UAITaskManager>(TEXT("AITaskManager"));
}

void AFirstBoss::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = GetMesh()->GetAnimInstance();
	check(AnimInstance);

	PrePosition = GetActorLocation();
}

float AFirstBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ResultDamage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

bool AFirstBoss::PlayAnimationFromData(const UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc)
{
	bool SaveResult = Super::PlayAnimationFromData(PatternData, EndFunc);
	if(SaveResult) StopMove();
	return SaveResult;
}

FRotator AFirstBoss::GetGroundMoveRot()
{
	if (GetActorLocation() == PrePosition)
	{
		return FRotator::ZeroRotator;
	}
	FRotator Rot = (GetActorLocation() - PrePosition).Rotation() - GetActorForwardVector().Rotation();
	PrePosition = GetActorLocation();
	return Rot;
}

void AFirstBoss::StartCurveMove(UCurveVector* CurveData)
{
	Super::StartCurveMove(CurveData);

	StopMove();
}

void AFirstBoss::PlayMontage(UAnimMontage* Montage, bool IsPrimitive)
{
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[FirstBoss] AnimInstance are not exist!!"));
		return;
	}

	if (IsPrimitive && AnimInstance->Montage_IsPlaying(nullptr)) return;

	StopMove();
	AnimInstance->Montage_Play(Montage);
}

void AFirstBoss::TurnToLoc(const FVector& TargetLocation)
{
	AITaskManager->TurnToLoc(TargetLocation);
}

void AFirstBoss::MoveToLoc(const FVector& TargetLocation, float InterpSpeed)
{
	AITaskManager->MoveToLoc(TargetLocation, InterpSpeed);
}

void AFirstBoss::MoveToLocUseMovementComponent(const FVector& TargetLocation)
{
	AITaskManager->StopMove();
	AITaskManager->MoveToLocUseMovementComponent(TargetLocation);
}

void AFirstBoss::StopMove()
{
	AITaskManager->StopMove();
}
