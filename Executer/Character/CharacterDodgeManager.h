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
	void InitManager(const float InGaugeMultiflier = 1.f, const float InDodgeRange = 150.f) 
	{
		GaugeMultiflier = InGaugeMultiflier; DodgeRange = InDodgeRange;
	}

// enable option section
public:
	UFUNCTION()
	void SetTickEnable(bool Enable);

// dynamic dodge section
public:
	UFUNCTION()
	void AddProjectileIdsToSet(const TSet<int32> NearProjectileIds);

// dynamic dodge section
private:
	UFUNCTION()
	TSet<int32> CheckNearProjectiles();

	UFUNCTION()
	void CheckDynamicDodge();

	UFUNCTION()
	void AddSpecialAttackGauge(const int32 ProjectileCount);

// dynamic dodge value section
private:
	UPROPERTY()
	TSet<int32> PreNearInstanceIds;

	UPROPERTY()
	TSet<int32> CurNearInstanceIds;

	UPROPERTY()
	float SpecialAttackGauge;

	UPROPERTY(EditAnywhere, Category = Dodge)
	float GaugeMultiflier;
		
	UPROPERTY()
	float DodgeRange;
};
