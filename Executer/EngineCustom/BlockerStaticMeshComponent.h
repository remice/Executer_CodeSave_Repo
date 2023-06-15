// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BlockerStaticMeshComponent.generated.h"

DECLARE_DELEGATE(FOnDeadSignature)

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup = (Rendering, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class EXECUTER_API UBlockerStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UBlockerStaticMeshComponent();

public:
	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	bool EvaluationDamage(int32 DamageLevel, float DamageAmount);
	
// Getter Setter Section
public:
	FORCEINLINE int32 GetBlockerLevel() const { return BlockerLevel; }
	FORCEINLINE void SetBlockerLevel(const int32 InLevel) { BlockerLevel = InLevel; }

	FORCEINLINE void SetMaxHp(const float InMaxHp) { MaxHp = InMaxHp; ChangeHp(MaxHp); }

private:
	void ChangeHp(const float InCurHp);

public:
	FOnDeadSignature OnDead;

private:
	int32 BlockerLevel = 0;

	float MaxHp;
	float CurHp;
};
