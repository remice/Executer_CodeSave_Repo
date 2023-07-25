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

	DodgeRange = 150.f;
	bIsDisable = false;
}


// Called when the game starts
void UCharacterDodgeManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UCharacterDodgeManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bIsDisable) return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckDynamicDodge();
}

void UCharacterDodgeManager::SetTickEnable(bool Enable)
{
	if (bIsDisable) return;

	CurNearInstanceIds.Empty();
	PreNearInstanceIds.Empty();

	PrimaryComponentTick.SetTickFunctionEnable(Enable);
}

void UCharacterDodgeManager::AddProjectileIdsToSet(const TSet<int32> NearProjectileIds)
{
	if (bIsDisable) return;

	CurNearInstanceIds.Append(NearProjectileIds);
}

void UCharacterDodgeManager::AddProjectileIdToSet(const int32& NearProjectileId)
{
	if (bIsDisable) return;

	CurNearInstanceIds.Emplace(NearProjectileId);
}

TSet<int32> UCharacterDodgeManager::CheckNearProjectiles()
{
	if (bIsDisable) return TSet<int32>();

	TSet<int32> ResultSet;

	// Collide section
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
		// 만약 회피 가능한 액터라면 결과 Set에 Id를 추가
		if (ICanBeDodgedActor* DodgedActor = Cast<ICanBeDodgedActor>(TraceHit.GetActor()))
		{
			ResultSet.Add(DodgedActor->GetId());
		}
	}
	// Collide section end

	return ResultSet;
}

void UCharacterDodgeManager::CheckDynamicDodge()
{
	if (bIsDisable) return;

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
	if (bIsDisable) return;

	if (ProjectileCount == 0)
	{
		return;
	}

	float DodgeAmount = GaugeMultiflier * ProjectileCount;

	APawn* PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn)
	{
		AExecuterPlayerState* PlayerState = PlayerPawn->GetPlayerState<AExecuterPlayerState>();
		if(PlayerState) PlayerState->GetDodged(DodgeAmount);
	}
}

void UCharacterDodgeManager::OnDodgeDisable(float CurHp, float MaxHp)
{
	if (bIsDisable) return;

	DisableDodgeTimerHandle.Invalidate();

	SetTickEnable(false);

	GetWorld()->GetTimerManager().SetTimer(DisableDodgeTimerHandle, this, &UCharacterDodgeManager::OnDodgeEnable, DodgeDisableTime, false);
}

void UCharacterDodgeManager::OnDodgeEnable()
{
	SetTickEnable(true);
	bIsDisable = false;
}
