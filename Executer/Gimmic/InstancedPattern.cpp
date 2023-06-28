// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedPattern.h"

#include "Interface/CanDodgeActor.h"
#include "Interface/IRotatableObject.h"
#include "Components/ArrowComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "EngineCustom/BlockerStaticMeshComponent.h"
#include "GameData/EXGameSingleton.h"

AInstancedPattern::AInstancedPattern()
{
	// Setup tick
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_PrePhysics);

	// Setup default instancedStaticMesh setting
	InstancedStaticMeshes = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("NAME_InstancedStaticMesh"));
	InstancedStaticMeshes->CastShadow = false;
	InstancedStaticMeshes->SetCollisionProfileName(TEXT("Projectile"));
	InstancedStaticMeshes->SetGenerateOverlapEvents(false);
	InstancedStaticMeshes->bMultiBodyOverlap = false;
	InstancedStaticMeshes->SetCanEverAffectNavigation(false);

	// Setup default pattern info
	AttackLevel = 0;
	BulletDamage = 50.f;
	DelayBetweenSpawn = 0.1f;
	NextDelay = 0.f;
	ComponentSize = 1.f;
	CollisionRadius = 30.f;
	ProjectileSpeed = 10.f;

	// Delay collision check values
	CheckLevel1Distance = 500.f;
	CheckLevel2Distance = 3000.f;
	CheckLevel3Distance = 5000.f;
	Level1CollisionCheckDelay = 0.25f;
	Level2CollisionCheckDelay = 5.f;
	NexLevel1CollisionCheckDelay = 0.f;
	NexLevel2CollisionCheckDelay = 0.f;
}

void AInstancedPattern::BeginPlay()
{
	Super::BeginPlay();

	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
}

void AInstancedPattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(TargetCharacter))
	{
		CharacterLocation = TargetCharacter->GetActorLocation();
	}
	else
	{
		CharacterLocation = FVector();
	}

	UpdateTransformInstanceMeshes();
}

// Setting bullet spawn
void AInstancedPattern::ExFire()
{
	APatternBase::ExFire();

	if (CurPatternTime < NextDelay)
	{
		return;
	}

	// Update next delay 
	NextDelay += DelayBetweenSpawn;

	SpawnBullets();
}

// Spawn bullets at locations
void AInstancedPattern::SpawnBullets()
{
	APatternBase::SpawnBullets();

	// If static mesh is nullptr, Do nothing
	if (InstancedStaticMeshes->GetStaticMesh() == nullptr)
	{
		return;
	}

	for (const auto FirePoint : FirePointArray)
	{
		IIRotatableObject* RotatableObject = Cast<IIRotatableObject>(FirePoint);

		if (RotatableObject)
		{
			RotatableObject->RelativeRotate(CurPatternTime);
		}

		FVector3d SpawnLoc = FirePoint->GetComponentLocation();
		FRotator3d SpawnRot = FirePoint->GetComponentRotation();
		FVector3d SpawnScale(ComponentSize);

		FTransform3d SpawnTrans(SpawnRot, SpawnLoc, SpawnScale);
		InstancedStaticMeshes->AddInstance(SpawnTrans, true);
	}
}

void AInstancedPattern::DestroyBullets()
{
	FTransform InstanceTransform;
	for (int ix = 0; ix < InstancedStaticMeshes->GetInstanceCount(); ++ix)
	{
		InstancedStaticMeshes->GetInstanceTransform(ix, InstanceTransform, true);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, InstanceTransform.GetLocation(), FRotator(0.f));
	}
	InstancedStaticMeshes->DestroyComponent();
}

void AInstancedPattern::OnCollideSomething(const FHitResult& HitResult, const FTransform& ComponentTransform)
{
	FEnemyAttackDataStruct AttackData = UEXGameSingleton::Get().EnemyAttackDataMap[EEnemyAttackData::Projectile];
	const FBaseEnemyAttackData* EnemyAttackData = AttackData.GetValidData<FBaseEnemyAttackData>(AttackLevel);
	float SingletonDamage = EnemyAttackData->Damage;

	FDamageEvent DamageEvent;
	HitResult.GetActor()->TakeDamage(SingletonDamage, DamageEvent, nullptr, this);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, ComponentTransform.GetLocation(), FRotator(0.f));

	UBlockerStaticMeshComponent* BlockerMeshComponent = Cast<UBlockerStaticMeshComponent>(HitResult.GetComponent());
	if (BlockerMeshComponent)
	{
		BlockerMeshComponent->EvaluationDamage(AttackLevel, SingletonDamage);
	}

	/* deprecate code
	FHitResult TraceHit;
	bool IsNearCharacter = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), ComponentTransform.GetLocation(),
		ComponentTransform.GetLocation(), 2500.f, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, TraceHit, true);

	if (IsNearCharacter && IsValid(HitEffect))
	{
		
	}
	*/
}

void AInstancedPattern::UpdateTransformInstanceMeshes()
{
	if (InstancedStaticMeshes->GetInstanceCount() == 0)
	{
		return;
	}
	
	TSet<int32> NearCharacterInstanceIds;
	FTransform ISMTrans;
	for (int ix = 0; ix < InstancedStaticMeshes->GetInstanceCount(); ++ix)
	{
		if (InstancedStaticMeshes->GetInstanceTransform(ix, ISMTrans, true) == false)
		{
			continue;
		}

		// Move instance
		ISMTrans.SetLocation(ISMTrans.GetLocation() + ISMTrans.Rotator().Quaternion().GetForwardVector() * ProjectileSpeed);
		InstancedStaticMeshes->UpdateInstanceTransform(ix, ISMTrans, true, true, true);

		// Calculate distance 'instance static mesh' to 'character'
		FVector DistanceToPlayer = ISMTrans.GetLocation() - CharacterLocation;
		float XAxisDistance = FMath::Abs(DistanceToPlayer.X);
		float YAxisDistance = FMath::Abs(DistanceToPlayer.Y);
		float ZAxisDistance = FMath::Abs(DistanceToPlayer.Z);

		bool bIsDistanceLevel3 = XAxisDistance > CheckLevel3Distance && YAxisDistance > CheckLevel3Distance && ZAxisDistance > CheckLevel3Distance;
		// if distance > check level 3, don't check collision
		if (bIsDistanceLevel3)
		{
			continue;
		}

		bool bIsDistanceLevel2 = XAxisDistance > CheckLevel2Distance && YAxisDistance > CheckLevel2Distance && ZAxisDistance > CheckLevel2Distance;
		// if distance > check level 2, check collision for every one second
		if (bIsDistanceLevel2)
		{
			if (CurPatternTime < NexLevel2CollisionCheckDelay)
			{
				continue;
			}
		}

		bool bIsDistanceLevel1 = XAxisDistance > CheckLevel1Distance && YAxisDistance > CheckLevel1Distance && ZAxisDistance > CheckLevel1Distance;
		// if distance > check level 1, check collision for every 1/4 second
		if (bIsDistanceLevel1)
		{
			if (CurPatternTime < NexLevel1CollisionCheckDelay)
			{
				continue;
			}
		}

		// Collision check
		FHitResult TraceHit;
		bool IsCollide = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), ISMTrans.GetLocation(), ISMTrans.GetLocation(),
			CollisionRadius * ComponentSize, CollisionObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, TraceHit, true);
		if (IsCollide)
		{
			OnCollideSomething(TraceHit, ISMTrans);
			DestroyInstanceIds.Emplace(ix);
			InstancedStaticMeshes->UpdateInstanceTransform(ix, FTransform(FRotator(0.f, 0.f, 0.f), FVector(0.f, 0.f, 100000.f)), true, true);
			continue;
		}

		bool bIsDistanceLevel0 = XAxisDistance < CheckLevel1Distance && YAxisDistance < CheckLevel1Distance && ZAxisDistance < CheckLevel1Distance;
		// if distance < check level 1, check whether exist character near to instance
		if (bIsDistanceLevel0 == false)
		{
			continue;
		}

		FHitResult Level0TraceHit;
		bool bExistNearCharacter = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), ISMTrans.GetLocation(), ISMTrans.GetLocation(),
			150.f, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, Level0TraceHit, true);

		if (bExistNearCharacter == false)
		{
			continue;
		}
		NearCharacterInstanceIds.Add(PatternId + ix);
	}

	// Update delay values
	if (CurPatternTime > NexLevel1CollisionCheckDelay)
	{
		NexLevel1CollisionCheckDelay += Level1CollisionCheckDelay;
	}

	if (CurPatternTime > NexLevel2CollisionCheckDelay)
	{
		NexLevel2CollisionCheckDelay += Level2CollisionCheckDelay;

		// Distroy instances for every Level2Delay
		if (DestroyInstanceIds.Num() != 0)
		{
			InstancedStaticMeshes->RemoveInstances(DestroyInstanceIds);
			DestroyInstanceIds.Empty();
		}
	}

	// Update character projectile set
	ICanDodgeActor* DodgeActor = Cast<ICanDodgeActor>(TargetCharacter);
	if (DodgeActor && NearCharacterInstanceIds.Num() != 0)
	{
		DodgeActor->AddProjectileIdsToSet(NearCharacterInstanceIds);
	}
}
