// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Skill/SkillSpawnUp.h"
#include "Components/StaticMeshComponent.h"
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
	BlockerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockerMesh"));
	SetRootComponent(RootSceneComponent);

	BlockerMesh->SetStaticMesh(M_SPHERE.Object);
	BlockerMesh->SetSimulatePhysics(false);
	BlockerMesh->SetEnableGravity(false);
	BlockerMesh->SetCollisionProfileName(TEXT("Blocker"));
	BlockerMesh->SetupAttachment(RootComponent);

	MovePoint = FVector::ZeroVector;
	MoveTime = 1.f;
	LifeDuration = 0.f;
	bOnMove = true;
}

// Called when the game starts or when spawned
void ASkillSpawnUp::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnNiagaraEffect();
}

// Called every frame
void ASkillSpawnUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeDuration += DeltaTime;
	MoveUpMesh();
}

void ASkillSpawnUp::SpawnNiagaraEffect()
{
	if (SpawnEffect == nullptr) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), GetActorRotation());
}

void ASkillSpawnUp::MoveUpMesh()
{
	if (bOnMove == false) return;

	float LerpAlpha = 0;
	if (MoveTime != 0)
	{
		LerpAlpha = LifeDuration / MoveTime;
	}

	if (LerpAlpha >= 1)
	{
		BlockerMesh->SetRelativeLocation(MovePoint);
		bOnMove = false;
		return;
	}

	FVector ModifyLoc = FMath::Lerp(FVector::ZeroVector, MovePoint, LerpAlpha);
	BlockerMesh->SetRelativeLocation(ModifyLoc);
}
