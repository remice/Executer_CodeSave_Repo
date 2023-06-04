// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Skill/SkillSpawnUp.h"
#include "EngineCustom/BlockerStaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#define PATH_SPHERE TEXT("/Engine/BasicShapes/Sphere.Sphere")

// Sets default values
ASkillSpawnUp::ASkillSpawnUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> M_SPHERE(PATH_SPHERE);
	check(M_SPHERE.Succeeded());

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	BlockerMesh = CreateDefaultSubobject<UBlockerStaticMeshComponent>(TEXT("BlockerMesh"));
	SetRootComponent(RootSceneComponent);

	BlockerMesh->SetStaticMesh(M_SPHERE.Object);
	BlockerMesh->SetSimulatePhysics(false);
	BlockerMesh->SetEnableGravity(false);
	BlockerMesh->SetCollisionProfileName(TEXT("Blocker"));
	BlockerMesh->SetupAttachment(RootComponent);

	MovePoint = FVector::ZeroVector;
	MoveTime = 1.f;
	LifeDuration = 0.f;
	MaxLifeTime = 5.f;
	bOnMove = true;
	bIsReverse = false;
	BlockerHp = 100.f;
}

// Called when the game starts or when spawned
void ASkillSpawnUp::BeginPlay()
{
	Super::BeginPlay();
	
	FOnDeadSignature DeadEvent;
	DeadEvent.BindLambda(
		[&] {
			Destroy();
		});
	BlockerMesh->OnDead = DeadEvent;
	BlockerMesh->SetMaxHp(BlockerHp);

	SpawnNiagaraEffect();
}

// Called every frame
void ASkillSpawnUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeDuration += DeltaTime;
	MoveUpMesh(bIsReverse);

	if (LifeDuration >= MaxLifeTime)
	{
		bOnMove = true;
		bIsReverse = true;
	}
}

void ASkillSpawnUp::SpawnNiagaraEffect()
{
	if (SpawnEffect == nullptr) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), GetActorRotation());
}

void ASkillSpawnUp::MoveUpMesh(bool IsReverse)
{
	if (bOnMove == false) return;

	float LerpAlpha = 0;
	float ModifiedTime = LifeDuration;

	if (IsReverse)
	{
		ModifiedTime -= MaxLifeTime;
	}

	if (MoveTime != 0)
	{
		LerpAlpha = ModifiedTime / MoveTime;
		LerpAlpha = IsReverse ? 1 - LerpAlpha : LerpAlpha;
	}

	if (LerpAlpha >= 1)
	{
		BlockerMesh->SetRelativeLocation(MovePoint);
		bOnMove = false;
		return;
	}
	else if (LerpAlpha <= -1)
	{
		BlockerMesh->SetRelativeLocation(FVector::ZeroVector);
		bOnMove = false;
		Destroy();
		return;
	}

	FVector ModifyLoc = FMath::Lerp(FVector::ZeroVector, MovePoint, LerpAlpha);
	BlockerMesh->SetRelativeLocation(ModifyLoc);
}
