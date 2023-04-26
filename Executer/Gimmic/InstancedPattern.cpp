// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedPattern.h"
#include "Interface/IRotatableObject.h"
#include "Character/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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
	BulletDamage = 50.f;
	CurPatternTime = 0.f;
	DelayBetweenSpawn = 0.1f;
	NextDelay = 0.f;
	MaxDelay = 7.f;
	ComponentSize = 1.f;
	CollisionRadius = 30.f;
	ProjectileSpeed = 10.f;

	// Delay collision check values
	CheckLevel1Distance = 500.f;
	CheckLevel2Distance = 2000.f;
	CheckLevel3Distance = 5000.f;
	Level1CollisionCheckDelay = 0.25f;
	Level2CollisionCheckDelay = 1.f;
	NexLevel1CollisionCheckDelay = 0.f;
	NexLevel2CollisionCheckDelay = 0.f;
}

void AInstancedPattern::BeginPlay()
{
	Super::BeginPlay();

	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	CollisionObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
}

// Tick life time = actor life time
void AInstancedPattern::Tick(float DeltaTime)
{
	// Check character isvalid & get character location
	APawn* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (IsValid(PlayerCharacter))
	{
		CharacterLocation = PlayerCharacter->GetActorLocation();
	}
	else
	{
		CharacterLocation = FVector();
	}

	UpdateTransformInstanceMeshes();
	// add delay time
	CurPatternTime += DeltaTime;

	if (bOnFire == false)
	{
		return;
	}

	if (CurPatternTime < NextDelay)
	{
		return;
	}

	ExFire();
}

// Setting bullet spawn
void AInstancedPattern::ExFire()
{
	// Update next delay 
	NextDelay += DelayBetweenSpawn;

	// Check end
	bool IsOverTime = NextDelay > MaxDelay;
	if (IsOverTime)
	{
		bOnFire = false;
		CurPatternTime = 0.f;
		NextDelay = 0.f;
		OnEnd.Execute();
		return;
	}

	// Select bullet
	int32 BulletIndex = PatternIndex;
	while (BulletIndex >= BulletArray.Num())
	{
		BulletIndex -= BulletArray.Num();
	}

	SpawnBullets(BulletIndex);
}

// Spawn bullets at locations
void AInstancedPattern::SpawnBullets(int32 BulletIndex)
{
	// If mesh class 
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

void AInstancedPattern::OnCollideSomething(const FHitResult& HitResult, const FTransform& ComponentTransform)
{
	APlayerCharacter* HitCharacter = Cast<APlayerCharacter>(HitResult.GetActor());
	if (IsValid(HitCharacter))
	{
		AExecuterPlayerState* HitPlayerState = Cast<AExecuterPlayerState>(HitCharacter->GetPlayerState());
		if (IsValid(HitPlayerState))
		{
			HitPlayerState->GetDamaged(BulletDamage);

			UE_LOG(LogTemp, Log, TEXT("%d"), (int32)HitPlayerState->GetHealth());
		}
	}

	FHitResult TraceHit;
	bool IsNearCharacter = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), ComponentTransform.GetLocation(),
		ComponentTransform.GetLocation(), 2500.f, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, TraceHit, true);

	if (IsNearCharacter && IsValid(HitEffect))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, ComponentTransform.GetLocation(), FRotator(0.f));
	}
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

		// Check distance level to check distances
		bool bIsDistanceLevel0 = XAxisDistance < CheckLevel1Distance && YAxisDistance < CheckLevel1Distance && ZAxisDistance < CheckLevel1Distance;
		bool bIsDistanceLevel1 = XAxisDistance > CheckLevel1Distance || YAxisDistance > CheckLevel1Distance || ZAxisDistance > CheckLevel1Distance;
		bool bIsDistanceLevel2 = XAxisDistance > CheckLevel2Distance || YAxisDistance > CheckLevel2Distance || ZAxisDistance > CheckLevel2Distance;
		bool bIsDistanceLevel3 = XAxisDistance > CheckLevel3Distance || YAxisDistance > CheckLevel3Distance || ZAxisDistance > CheckLevel3Distance;

		// if distance > check level 3, don't check collision
		if (bIsDistanceLevel3)
		{
			continue;
		}

		// if distance > check level 2, check collision for every one second
		if (bIsDistanceLevel2)
		{
			if (CurPatternTime < NexLevel2CollisionCheckDelay)
			{
				continue;
			}
		}

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
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter && NearCharacterInstanceIds.Num() != 0)
	{
		PlayerCharacter->AddProjectileIdsToSet(NearCharacterInstanceIds);
	}
}
