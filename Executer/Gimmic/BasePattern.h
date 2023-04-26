// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePattern.generated.h"

DECLARE_DELEGATE(FPatternOnEndSignature)

UCLASS()
class EXECUTER_API ABasePattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePattern();

	FPatternOnEndSignature OnEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	float CurPatternTime;

	int32 PatternIndex;

	uint8 bOnFire:1;

	UPROPERTY(EditAnywhere, Category=Pattern)
	TArray<TSubclassOf<class AProjectile>> BulletArray;

	UPROPERTY(EditAnywhere, Category = Pattern)
	TArray<TObjectPtr<class UArrowComponent>> FirePointArray;

	UPROPERTY(EditAnywhere, Category=Pattern)
	TArray<float> DelayArray;

	UPROPERTY(EditAnywhere, Category = Pattern)
	int32 PatternId;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Body;

public:
	UFUNCTION()
	virtual void Fire();

protected:
	UFUNCTION()
	virtual void ExFire();

	UFUNCTION()
	virtual void SpawnBullets(int32 BulletIndex);
};
