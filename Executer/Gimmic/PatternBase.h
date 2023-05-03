// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatternBase.generated.h"

DECLARE_DELEGATE(FPatternOnEndSignature)

UCLASS()
class EXECUTER_API APatternBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatternBase();

	FPatternOnEndSignature OnEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetupPattern(AActor* InPlayerCharacter);

	UFUNCTION()
	virtual void Fire();

	FORCEINLINE void SetPatternId(const float& InPatternId) { PatternId = InPatternId; }

protected:
	UPROPERTY(EditAnywhere, Category = Pattern)
	float MaxPatternTime;

	UPROPERTY(EditAnywhere, Category = Pattern)
	float DestroyTime;

	float CurPatternTime;
	uint8 bOnFire : 1;

	UPROPERTY(EditAnywhere, Category = Pattern)
	TArray<TObjectPtr<class UArrowComponent>> FirePointArray;

	UPROPERTY(EditAnywhere, Category = Pattern)
	int32 PatternId;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Body;

	TObjectPtr<AActor> PlayerCharacter;

protected:
	UFUNCTION()
	virtual void ExFire();

	UFUNCTION()
	virtual void SpawnBullets();

};
