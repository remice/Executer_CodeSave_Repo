// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossBase.h"
#include "Kismet/GameplayStatics.h"
#include "Gimmic/PatternBase.h"
#include "Components/SkeletalMeshComponent.h"

ABossBase::ABossBase()
{
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ensure(Mesh);

	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetupAttachment(Collider);
	Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);
}

void ABossBase::SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName)
{
	if (IsValid(NewPatternClass) == false)
	{
		return;
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
	APatternBase* NewPatternManager= GetWorld()->SpawnActor<APatternBase>(NewPatternClass, SpawnPos, SpawnRot, SpawnParameters);
	if (NewPatternManager)
	{
		NewPatternManager->SetupPattern(PlayerActor);
		if (Mesh->DoesSocketExist(SocketName))
		{
			NewPatternManager->AttachToComponent(Mesh, AttachmentRules, SocketName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss SocketName doesn't exist!!"));
			NewPatternManager->AttachToActor(this, AttachmentRules, NAME_None);
		}
		//NewPatternManager->OnEnd.BindUFunction(this, TEXT("ExNextPattern"));
		//PatternManager->OnEnd.BindSP(this, &ATutorialRanger::ExNextPattern);
		//PatternManager->OnEnd.BindUObject(this, &ATutorialRanger::ExNextPattern);
		NewPatternManager->Fire();
	}
}
