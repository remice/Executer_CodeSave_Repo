// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/Lasor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "Interface/CanDodgeActor.h"

ALasor::ALasor()
{
	PrimaryActorTick.bCanEverTick = true;

	LasorEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LasorEffect"));
	SetRootComponent(LasorEffectComponent);

	LasorEndPoint = FLasorValueVector(TEXT("End"), FVector::ZeroVector);
	LasorWidth = FLasorValueFloat(TEXT("Width"), 30.f);
	AttackDelay = 0.1f;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

	DodgeObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	SetLifeSpan(5.f);

	AttackType = EEnemyAttackData::Lasor;
	InterpSpeed = 20.f;
}

void ALasor::BeginPlay()
{
	Super::BeginPlay();

	if (LasorEffectComponent->GetAsset() == nullptr)
	{
		return;
	}

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

	TargetLocation = Target->GetActorLocation();

	LasorEffectComponent->ActivateSystem();
	LasorEffectComponent->SetNiagaraVariableFloat(LasorWidth.Name, LasorWidth.Value);
}

void ALasor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LasorEffectComponent->GetAsset() == nullptr)
	{
		return;
	}

	RotateLasor(DeltaTime);
	ExLasor();
	UpdateLasor();
	DodgeLasor();
}

void ALasor::RotateLasor(float DeltaTime)
{
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(GetActorRotation(), LookRotation, DeltaTime, InterpSpeed);
	SetActorRotation(NewRotation);
}

void ALasor::ExLasor()
{
	FHitResult HitResult;
	bool bOnCollide = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 5000.f,
		LasorWidth.Value * 0.5f, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, HitResult, true);

	if (bOnCollide == false)
	{
		LasorEndPoint.Value = GetActorLocation() + GetActorForwardVector() * 5000.f;
		return;
	}

	APawn* HitPawn = Cast<APawn>(HitResult.GetActor());

	if (HitPawn == nullptr)
	{
		FVector Error = GetActorLocation() - HitResult.GetActor()->GetActorLocation();
		LasorEndPoint.Value = GetActorLocation() + GetActorForwardVector() * Error.Size();
		return;
	}

	FDamageEvent DamageEvent;
	HitPawn->TakeDamage(GetDamage(), DamageEvent, nullptr, this);
	IgnoreActorArray.Emplace(HitResult.GetActor());

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ALasor::ResetIgnoreActorArray, AttackDelay, false, AttackDelay);
}

void ALasor::UpdateLasor()
{
	LasorEffectComponent->SetNiagaraVariableVec3(LasorEndPoint.Name, LasorEndPoint.Value);
}

void ALasor::DodgeLasor()
{
	FHitResult PawnHitResult;
	bool bOnCollidePawn = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetActorLocation(), LasorEndPoint.Value,
		150.f, DodgeObjectTypesArray, false, IgnoreDodgeActorArray, EDrawDebugTrace::None, PawnHitResult, true);

	if (bOnCollidePawn == false) return;

	ICanDodgeActor* DodgePawn = Cast<ICanDodgeActor>(PawnHitResult.GetActor());
	if (DodgePawn)
	{
		DodgePawn->AddProjectileIdToSet(Id);
		IgnoreDodgeActorArray.Emplace(PawnHitResult.GetActor());

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ALasor::ResetIgnoreDodgeActorArray, AttackDelay, false, AttackDelay);
	}
}
