// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Projectile_CosmicBomb.h"

#include "Curves/CurveVector.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DamageEvents.h"
#include "EngineCustom/BlockerStaticMeshComponent.h"

AProjectile_CosmicBomb::AProjectile_CosmicBomb()
{
	PrimaryActorTick.bCanEverTick = true;

	SetLifeSpan(10.f);
	rAttackType = EAttackData::Bomb;
}

void AProjectile_CosmicBomb::BeginPlay()
{
	Super::BeginPlay();

	GetBody()->SetSimulatePhysics(false);
	GetBody()->SetGenerateOverlapEvents(false);
	GetProjectileMovement()->bAutoActivate = false;
	GetProjectileMovement()->SetActive(false);

	if (MoveCurve == nullptr || ScaleCurve == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AProjectile_CosmicBomb] Curve data are not exist"));
		Destroy();
	}

	double MinTime;
	MoveCurve->GetTimeRange(MinTime, MaxMoveCurveTime);
	MaxMoveCurveTime -= MinTime;

	ScaleCurve->GetTimeRange(MinTime, MaxScaleCurveTime);
	MaxScaleCurveTime -= MinTime;

	SavePos = GetActorLocation();

	CurTime = 0;
	AttackSphereRadius = 100.f;
}

void AProjectile_CosmicBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurTime += DeltaTime;

	FVector MoveVector = MoveCurve->GetVectorValue(CurTime);
	float ScaleFloat = ScaleCurve->GetFloatValue(CurTime);

	FVector ForwardVector = GetActorForwardVector();
	FVector ScaledMoveVector = MoveVector;
	ScaledMoveVector.X = MoveVector.X * ForwardVector.X + MoveVector.Y * ForwardVector.Y;
	ScaledMoveVector.Y = MoveVector.X * ForwardVector.Y + MoveVector.Y * ForwardVector.X;

	SetActorLocation(SavePos + ScaledMoveVector);
	SetActorScale3D(FVector(ScaleFloat, ScaleFloat, ScaleFloat));

	if (CurTime > MaxMoveCurveTime)
	{
		SpawnHitEffect();
		Explosion();
		SetActorTickEnabled(false);
		Destroy();
	}
}

void AProjectile_CosmicBomb::Explosion()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeArray;
	ObjectTypeArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypeArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectTypeArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	TArray<AActor*> IgnoreActorArray;
	TArray<AActor*> OuterArray;
	bool IsCollide = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), AttackSphereRadius, 
		ObjectTypeArray, nullptr, IgnoreActorArray, OuterArray);

	if (IsCollide)
	{
		for (auto OuterActor : OuterArray)
		{
			UBlockerStaticMeshComponent* BlockerMeshComponent = Cast<UBlockerStaticMeshComponent>(OuterActor->GetComponentByClass(UBlockerStaticMeshComponent::StaticClass()));
			if (BlockerMeshComponent)
			{
				BlockerMeshComponent->EvaluationDamage(GetAttackLevel(), GetDamage());
			}

			FDamageEvent DamageEvent;
			OuterActor->TakeDamage(GetDamage(), DamageEvent, nullptr, this);
		}
	}
}
