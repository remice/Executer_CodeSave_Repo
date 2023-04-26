// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "BaseEnemy.generated.h"

UCLASS()
class EXECUTER_API ABaseEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, Category=Character)
	TObjectPtr<UCapsuleComponent> Collider;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float HP;

public:
	UFUNCTION()
	FORCEINLINE UCapsuleComponent* GetCollider() const { return Collider; }
	
	UFUNCTION()
	FORCEINLINE float GetMaxHP() const { return MaxHP; }

	UFUNCTION()
	FORCEINLINE float GetHP() const { return HP; }

	UFUNCTION()
	virtual float GetDamage(float InDamage);
};
