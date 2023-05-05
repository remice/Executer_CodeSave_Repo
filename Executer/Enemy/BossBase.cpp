// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossBase.h"
#include "Kismet/GameplayStatics.h"
#include "Gimmic/PatternBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/BossPatternData.h"

ABossBase::ABossBase()
{
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	ensure(Mesh);

	Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	Mesh->bCastDynamicShadow = true;
	Mesh->bAffectDynamicIndirectLighting = true;
	Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Mesh->SetupAttachment(Collider);
	Mesh->SetCollisionProfileName(TEXT("Boss"));
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCanEverAffectNavigation(false);
	Mesh->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
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
	FRotator3d SpawnRot = RootComponent->GetComponentRotation();
	if (Mesh->DoesSocketExist(SocketName))
	{
		SpawnPos = Mesh->GetSocketLocation(SocketName);
		SpawnRot = Mesh->GetSocketRotation(SocketName);
	}

	// Spawn & attachment rules
	FActorSpawnParameters SpawnParameters;
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;

	// Spawn pattern and execute attack
	APatternBase* NewPatternManager= GetWorld()->SpawnActor<APatternBase>(NewPatternClass, SpawnPos, SpawnRot, SpawnParameters);
	if (NewPatternManager)
	{
		NewPatternManager->SetupPattern(PlayerActor);
		NewPatternManager->AttachToActor(this, AttachmentRules, NAME_None);
		//NewPatternManager->OnEnd.BindUFunction(this, TEXT("ExNextPattern"));
		//PatternManager->OnEnd.BindSP(this, &ATutorialRanger::ExNextPattern);
		//PatternManager->OnEnd.BindUObject(this, &ATutorialRanger::ExNextPattern);
		NewPatternManager->Fire();
	}
}

void ABossBase::PlayAnimationFromData(const UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc)
{
	UAnimMontage* Montage = PatternData->LinkAnimationMontage;
	if (IsValid(Montage) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss AI Pattern data doesn't have montage!!"));
	}

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return;
	}

	float CalcPlayRate = AnimPlayRate * PatternData->PlayRate;
	AnimInstance->Montage_Play(Montage, CalcPlayRate);

	EndAnimationDelegate = EndFunc;
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABossBase::EndAnimation);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
}

void ABossBase::StopAnimation()
{
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return;
	}

	AnimInstance->Montage_Stop(0.f);
}

void ABossBase::EndAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	EndAnimationDelegate.ExecuteIfBound();
}
