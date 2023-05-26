// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterDodgeManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/CanBeDodgedActor.h"
#include "Player/ExecuterPlayerState.h"

// Sets default values for this component's properties
UCharacterDodgeManager::UCharacterDodgeManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SpecialAttackGauge = 0.f;
	GaugeMultiflier = 1.f;
	DodgeRange = 150.f;
}


// Called when the game starts
void UCharacterDodgeManager::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UCharacterDodgeManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckDynamicDodge();
}

void UCharacterDodgeManager::SetTickEnable(bool Enable)
{
	CurNearInstanceIds.Empty();
	PreNearInstanceIds.Empty();

	PrimaryComponentTick.SetTickFunctionEnable(Enable);
}

void UCharacterDodgeManager::AddProjectileIdsToSet(const TSet<int32> NearProjectileIds)
{
	CurNearInstanceIds.Append(NearProjectileIds);
}

TSet<int32> UCharacterDodgeManager::CheckNearProjectiles()
{
	check(IsValid(GetOwner()));

	TSet<int32> ResultSet;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActorArray;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	
	TArray<FHitResult> TraceHits;
	bool IsNearCharacter = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation(), 150.f, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, TraceHits, true);

	for (const auto TraceHit : TraceHits)
	{
		if (ICanBeDodgedActor* DodgedActor = Cast<ICanBeDodgedActor>(TraceHit.GetActor()))
		{
			ResultSet.Add(DodgedActor->GetId());
		}
	}

	return ResultSet;
}

void UCharacterDodgeManager::CheckDynamicDodge()
{
	// Update set
	CurNearInstanceIds.Append(CheckNearProjectiles());

	TSet<int32> DodgeProjectileIds = PreNearInstanceIds.Difference(CurNearInstanceIds);

	if (DodgeProjectileIds.Num() != 0)
	{
		AddSpecialAttackGauge(DodgeProjectileIds.Num());
	}

	// Reset CheckData
	PreNearInstanceIds.Empty();
	PreNearInstanceIds.Append(CurNearInstanceIds);
	CurNearInstanceIds.Empty();
}

void UCharacterDodgeManager::AddSpecialAttackGauge(const int32 ProjectileCount)
{
	if (ProjectileCount == 0)
	{
		return;
	}

	SpecialAttackGauge += GaugeMultiflier * ProjectileCount;

	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn)
	{
		AExecuterPlayerState* PlayerState = PlayerPawn->GetPlayerState<AExecuterPlayerState>();

		PlayerState->SetupSpecial(SpecialAttackGauge);
	}
}

void UCharacterDodgeManager::OnDodgeDisable(float CurHp, float MaxHp)
{
	DisableDodgeTimerHandle.Invalidate();

	SetTickEnable(false);

	GetWorld()->GetTimerManager().SetTimer(DisableDodgeTimerHandle, this, &UCharacterDodgeManager::OnDodgeEnable, DodgeDisableTime, false);
}

void UCharacterDodgeManager::OnDodgeEnable()
{
	SetTickEnable(true);
}
