// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossBase.h"

#include "Kismet/GameplayStatics.h"
#include "Gimmic/PatternBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemy/BossPatternData.h"
#include "Curves/CurveVector.h"

ABossBase::ABossBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->bAffectDynamicIndirectLighting = true;
	GetMesh()->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	GetMesh()->SetupAttachment(GetCapsuleComponent());
	GetMesh()->SetCollisionProfileName(TEXT("Boss"));
	GetMesh()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCanEverAffectNavigation(false);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	GetCapsuleComponent()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetEnableGravity(true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Boss"));

	bOnCurve = false;
	SaveLocation = FVector();
	SaveForwardVector = FVector();
}

void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnCurve)
	{
		RunCurveMove();
	}
}

void ABossBase::SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName, bool bAttachLocation, bool bAttachRotation)
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
	if (GetMesh()->DoesSocketExist(SocketName))
	{
		SpawnPos = GetMesh()->GetSocketLocation(SocketName);
		SpawnRot = GetMesh()->GetSocketRotation(SocketName);
	}

	// Spawn & attachment rules
	FActorSpawnParameters SpawnParameters;
	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;

	if (bAttachLocation)
	{
		AttachmentRules.LocationRule = EAttachmentRule::SnapToTarget;
	}

	if (bAttachRotation)
	{
		AttachmentRules.RotationRule = EAttachmentRule::SnapToTarget;
	}

	// Spawn pattern and execute attack
	APatternBase* NewPatternManager= GetWorld()->SpawnActor<APatternBase>(NewPatternClass, SpawnPos, SpawnRot, SpawnParameters);
	if (NewPatternManager)
	{
		NewPatternManager->SetupPattern(PlayerActor);
		if (GetMesh()->DoesSocketExist(SocketName))
		{
			NewPatternManager->AttachToComponent(GetMesh(), AttachmentRules, SocketName);
		}
		else
		{
			NewPatternManager->AttachToActor(this, AttachmentRules, NAME_None);
		}

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

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return;
	}

	float CalcPlayRate = AnimPlayRate * PatternData->PlayRate;
	AnimInstance->Montage_Play(Montage, CalcPlayRate);

	if (PatternData->bOnMove)
	{
		PlayRate = PatternData->PlayRate;
		StartCurveMove(PatternData->MoveCurve);
	}

	EndAnimationDelegate = EndFunc;
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABossBase::EndAnimation);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
}

void ABossBase::StopAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return;
	}

	AnimInstance->Montage_Stop(0.f);
}

void ABossBase::EndAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	EndCurveMove();

	EndAnimationDelegate.ExecuteIfBound();
}

void ABossBase::StartCurveMove(UCurveVector* CurveData)
{
	if (IsValid(CurveData) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Anim Notify] bOnMove is True, but CurveData was Nullptr!!"));
		return;
	}
	
	AnimCurveData = CurveData;
	ensure(AnimCurveData);

	SaveLocation = FVector::ZeroVector;
	GetCapsuleComponent()->SetEnableGravity(false);
	bOnCurve = true;
}

void ABossBase::RunCurveMove()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return;
	}

	float AnimPosition = AnimInstance->Montage_GetPosition(nullptr) * PlayRate;
	
	if (AnimPosition < SMALL_NUMBER)
	{
		return;
	}

	SaveForwardVector = GetActorForwardVector();
	SaveForwardVector.Z = 0;
	SaveForwardVector.Normalize();

	FVector CurveLocation = AnimCurveData->GetVectorValue(AnimPosition);
	FVector ActualLocation = FVector(CurveLocation.X * SaveForwardVector.X, CurveLocation.X * SaveForwardVector.Y, CurveLocation.Z);
	FVector DeltaLocation = ActualLocation - SaveLocation;
	SaveLocation = ActualLocation;
	SetActorLocation(GetActorLocation() + DeltaLocation, true);
}

void ABossBase::EndCurveMove()
{
	GetCapsuleComponent()->SetEnableGravity(true);
	bOnCurve = false;
}
