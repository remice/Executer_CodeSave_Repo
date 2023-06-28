// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "Interface/PatternSpawnable.h"
#include "Interface/AnimationTaskable.h"

#include "BossBase.generated.h"

USTRUCT()
struct FBadState_Base
{
	GENERATED_BODY()
	
public:
	FBadState_Base() : Owner(nullptr) {}
	FBadState_Base(ABossBase* InOwner) : Owner(InOwner) {}
	virtual ~FBadState_Base() {
		Owner = nullptr;
	}

	FORCEINLINE void SetOwner(ABossBase* InOwner) { Owner = InOwner; }

	virtual void OnState() {}
	virtual void OffState() {}

protected:
	UPROPERTY()
	TWeakObjectPtr<ABossBase> Owner;

	FTimerHandle TimerHandle;
};

USTRUCT()
struct FBadState_Stun : public FBadState_Base
{
	GENERATED_BODY()

public:
	FBadState_Stun(float InStunTime = 1.f, ABossBase* InOwner = nullptr) : FBadState_Base(InOwner), StunTime(InStunTime) {}
	virtual ~FBadState_Stun() override {
		FBadState_Base::~FBadState_Base();
		Owner = nullptr;
	}

	virtual void OnState() override;
	virtual void OffState() override;

private:
	float StunTime;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API ABossBase : public ABaseEnemy, public IPatternSpawnable, public IAnimationTaskable
{
	GENERATED_BODY()
	
public:
	ABossBase();

	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

// Interface Section
public:
	virtual void SpawnPatternManager(TSubclassOf<APatternBase> NewPatternClass, FName SocketName, bool bAttachLocation, bool bAttachRotation) override;

	virtual bool PlayAnimationFromData(const class UBossPatternData* PatternData, const FOnEndAnimationSigniture& EndFunc) override;
	virtual void StopAnimation() override;
	void StopAllPattern(bool bIsDestroyBullet);

	virtual void ChangeBadState(FBadState_Base* StateStruct);
	virtual void OnStun();
	virtual void EndStun();

protected:
	void EndAnimation(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

// Curve Movement Section
protected:
	virtual void StartCurveMove(class UCurveVector* CurveData);
	void RunCurveMove();
	void EndCurveMove();

protected:
	float AnimPlayRate = 1.0f;

	FOnEndAnimationSigniture EndAnimationDelegate;

// Curve Movement Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<class UCurveVector> AnimCurveData;

	uint8 bOnCurve : 1;
	float PlayRate = 1.f;
	FVector SaveLocation;
	FVector SaveForwardVector;

	UPROPERTY()
	TArray<TWeakObjectPtr<class APatternBase>> PatternManagerArray;

	FBadState_Base* BadState;

protected:
	class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};
