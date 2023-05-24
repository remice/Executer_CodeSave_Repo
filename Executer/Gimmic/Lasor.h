// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/AttackBase.h"
#include "Lasor.generated.h"

USTRUCT()
struct FLasorValueVector
{
	GENERATED_BODY()
	
public:
	FLasorValueVector() : Name(), Value() {}
	FLasorValueVector(FString InName, FVector InValue) : Name(InName), Value(InValue) {}

public:
	UPROPERTY(EditAnywhere, Category="Property")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Property")
	FVector Value;
};

USTRUCT()
struct FLasorValueFloat
{
	GENERATED_BODY()

public:
	FLasorValueFloat() : Name(), Value() {}
	FLasorValueFloat(FString InName, float InValue) : Name(InName), Value(InValue) {}

public:
	UPROPERTY(EditAnywhere, Category = "Property")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Property")
	float Value;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API ALasor : public AAttackBase
{
	GENERATED_BODY()
	
public:
	ALasor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void RotateLasor(float DeltaTime);
	virtual void ExLasor();
	virtual void UpdateLasor();
	
protected:
	UFUNCTION()
	void ResetIgnoreActorArray() {IgnoreActorArray.Empty(); }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraComponent> LasorEffectComponent;

	UPROPERTY(EditAnywhere, Category = "Property")
	FLasorValueVector LasorEndPoint;

	UPROPERTY(EditAnywhere, Category = "Property")
	FLasorValueFloat LasorWidth;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActorArray;

	FVector TargetLocation;
	float AttackDelay;
};
