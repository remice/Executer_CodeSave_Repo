// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterDodgeManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UCharacterDodgeManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterDodgeManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void InitManager(const float InGaugeMultiflier = 1.f, const float InDodgeRange = 150.f, const float InDodgeDisableTime = 1.f) 
	{
		GaugeMultiflier = InGaugeMultiflier; DodgeRange = InDodgeRange; DodgeDisableTime = InDodgeDisableTime;
	}
	void SetDisableManager() { bIsDisable = false; }

// enable option section
public:
	UFUNCTION()
	void OnDodgeDisable(float CurHp, float MaxHp);

// dynamic dodge section
public:
	UFUNCTION()
	void AddProjectileIdsToSet(const TSet<int32> NearProjectileIds);
	void AddProjectileIdToSet(const int32& NearProjectileId);

// dynamic dodge section
private:
	UFUNCTION()
	TSet<int32> CheckNearProjectiles();

	UFUNCTION()
	void CheckDynamicDodge();

	UFUNCTION()
	void AddSpecialAttackGauge(const int32 ProjectileCount);

	UFUNCTION()
	void SetTickEnable(bool Enable);

	UFUNCTION()
	void OnDodgeEnable();

// dynamic dodge value section
private:
	UPROPERTY()
	TSet<int32> PreNearInstanceIds;

	UPROPERTY()
	TSet<int32> CurNearInstanceIds;

	UPROPERTY()
	float GaugeMultiflier = 5.f;
		
	UPROPERTY()
	float DodgeRange;

	FTimerHandle DisableDodgeTimerHandle;
	float DodgeDisableTime = 1.f;

	uint8 bIsDisable : 1;
};
