// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossBase.h"

#include "Game/MyGameInstance.h"
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
	bOnStun = false;
	SaveLocation = FVector();
	SaveForwardVector = FVector();
}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();

	BadState = new FBadState_Base(this);
}

void ABossBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnStun) return;

	if (bOnCurve)
	{
		RunCurveMove();
	}
}

void ABossBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->SetMapBoss(this);
	}

	HP = MaxHP;
}

void ABossBase::SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName, bool bAttachLocation, bool bAttachRotation)
{
	if (bOnStun) return;

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
		PatternManagerArray.Emplace(NewPatternManager);
	}
}

bool ABossBase::PlayAnimationFromData(const UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc)
{
	if (bOnStun) return false;

	UAnimMontage* Montage = PatternData->LinkAnimationMontage;
	if (IsValid(Montage) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss AI Pattern data doesn't have montage!!"));
		return false;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Anim Instance doesn't exist!!"));
		return false;
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
	return true;
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

void ABossBase::ChangeBadState(FBadState_Base* StateStruct)
{
	if (BadState)
	{
		BadState->OffState();
		delete BadState;
	}
	BadState = StateStruct;
	BadState->SetOwner(this);
	BadState->OnState();
}

void ABossBase::OnStun()
{
	bOnStun = true;

	return;
}

void ABossBase::EndAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	EndCurveMove();

	EndAnimationDelegate.ExecuteIfBound();
}

void ABossBase::EndStun()
{
	bOnStun = false;

	return;
}

void ABossBase::StopAllPattern(bool bIsDestroyBullet)
{
	for (const auto PatternManager : PatternManagerArray)
	{
		if (PatternManager.IsValid())
			PatternManager.Get()->StopPattern(bIsDestroyBullet);
	}
	PatternManagerArray.Empty();
}

void ABossBase::StartCurveMove(UCurveVector* CurveData)
{
	if (bOnStun) return;

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
	if (bOnStun) return;

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
	if (bOnStun) return;

	GetCapsuleComponent()->SetEnableGravity(true);
	bOnCurve = false;
}

// FBadeState struct section
void FBadState_Stun::OnState()
{
	if (Owner.IsValid() == false) return;
	Owner.Get()->StopAllPattern(true);
	Owner.Get()->OnStun();

	FTimerDelegate EndStunDelegate;
	EndStunDelegate.BindLambda(
		[&]() {
			if (Owner.IsValid())
			{
				FBadState_Base* NormalState = new FBadState_Base();
				Owner.Get()->ChangeBadState(NormalState);
			}
		});

	Owner.Get()->GetWorldTimerManager().SetTimer(TimerHandle, EndStunDelegate, 0.1f, false, StunTime);
}

void FBadState_Stun::OffState()
{
	if (Owner.IsValid() == false) return;
	Owner.Get()->EndStun();
	Owner.Get()->GetWorldTimerManager().ClearTimer(TimerHandle);
}
