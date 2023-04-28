// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialRanger.h"
#include "Gimmic/BasePattern.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

#define PATH_SPHERE TEXT("/Engine/BasicShapes/Sphere.Sphere")

ATutorialRanger::ATutorialRanger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create default object
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	// Set mesh & attachment
	ConstructorHelpers::FObjectFinder<UStaticMesh> M_SPHERE(PATH_SPHERE);
	check(M_SPHERE.Succeeded());

	Mesh->SetStaticMesh(M_SPHERE.Object);
	Mesh->SetupAttachment(GetCollider());

	Arrow->SetupAttachment(Mesh);
	Arrow->SetRelativeLocation(FVector3d(40.f, 0.f, 0.f));

	CurDelay = 0.f;
	DelayIndex = 0;
	bOnPattern = false;
}

void ATutorialRanger::BeginPlay()
{
	Super::BeginPlay();
}

void ATutorialRanger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool bOverDelayArrayIndex = DelayIndex >= DelayArray.Num();
	if (bOnPattern || bOverDelayArrayIndex)
	{
		return;
	}

	CurDelay += DeltaTime;
	if (CurDelay < DelayArray[DelayIndex])
	{
		return;
	}

	if (IsValid(PatternClass[DelayIndex]) == false)
	{
		return;
	}

	SpawnPatternManager(PatternClass[DelayIndex]);
}


void ATutorialRanger::ExNextPattern()
{
	bOnPattern = false;
}

void ATutorialRanger::SpawnPatternManager(TSubclassOf<ABasePattern> NewPatternClass)
{
	bOnPattern = true;
	CurDelay = 0.f;
	DelayIndex++;

	if (IsValid(NewPatternClass) == false)
	{
		return;
	}

	if (IsValid(PatternManager))
	{
		PatternManager->SetLifeSpan(30.f);
		PatternManager = nullptr;
	}

	AActor* PlayerActor = Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (IsValid(PlayerActor) == false)
	{
		return;
	}

	// Spawn bullet
	FVector3d SpawnPos = RootComponent->GetComponentLocation();
	SpawnPos += FVector3d(70.f, 0.f, 0.f);
	FRotator3d SpawnRot = RootComponent->GetComponentRotation();

	// Spawn & attachment rules
	FActorSpawnParameters SpawnParameters;
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;

	// Spawn pattern and execute attack
	PatternManager = GetWorld()->SpawnActor<ABasePattern>(NewPatternClass, SpawnPos, SpawnRot, SpawnParameters);
	if (PatternManager)
	{
		PatternManager->SetupPattern(PlayerActor);
		PatternManager->AttachToActor(this, AttachmentRules, NAME_None);
		PatternManager->OnEnd.BindUFunction(this, TEXT("ExNextPattern"));
		//PatternManager->OnEnd.BindSP(this, &ATutorialRanger::ExNextPattern);
		PatternManager->Fire();
	}
	//AProjectile* TempBullet = GetWorld()->SpawnActor<AProjectile>(AProjectile::StaticClass(), SpawnPos, SpawnRot, SpawnParameters);
}
