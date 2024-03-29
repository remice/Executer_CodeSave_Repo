// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmic/BaseAttack.h"
#include "Lasor.generated.h"

USTRUCT()
struct FLasorValueVector
{
	GENERATED_BODY()
	
public:
	FLasorValueVector() {}
	FLasorValueVector(FString InName, FVector InValue) : Name(InName), Value(InValue) {}

public:
	UPROPERTY(EditAnywhere, Category="Property")
	FString Name = FString();

	UPROPERTY(EditAnywhere, Category = "Property")
	FVector Value = FVector();
};

USTRUCT()
struct FLasorValueFloat
{
	GENERATED_BODY()

public:
	FLasorValueFloat() {}
	FLasorValueFloat(FString InName, float InValue) : Name(InName), Value(InValue) {}

public:
	UPROPERTY(EditAnywhere, Category = "Property")
	FString Name = FString();

	UPROPERTY(EditAnywhere, Category = "Property")
	float Value = 0;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API ALasor : public ABaseAttack
{
	GENERATED_BODY()
	
public:
	ALasor();

	virtual void DestroySelf() override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void RotateLasor(float DeltaTime);
	virtual void ExLasor();
	virtual void UpdateLasor();
	virtual void DodgeLasor();
	
protected:
	UFUNCTION()
	void ResetIgnoreActorArray() {IgnoreActorArray.Empty(); }
	UFUNCTION()
	void ResetIgnoreDodgeActorArray() { IgnoreDodgeActorArray.Empty(); }
	
protected:
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<class UNiagaraComponent> LasorEffectComponent;

	UPROPERTY(EditAnywhere, Category = "Property")
	FLasorValueVector LasorEndPoint;

	UPROPERTY(EditAnywhere, Category = "Property")
	FLasorValueFloat LasorWidth;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float InterpSpeed;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreActorArray;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> DodgeObjectTypesArray;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoreDodgeActorArray;

	FVector TargetLocation;
	float AttackDelay;
};
