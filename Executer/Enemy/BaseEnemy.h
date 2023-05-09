// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "BaseEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedSignature, float /*CurrenHp*/)

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, Category=Character)
	TObjectPtr<UBoxComponent> Collider;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float HP;

public:
	FORCEINLINE UBoxComponent* GetCollider() const { return Collider; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetHP() const { return HP; }

	FOnHpChangedSignature OnHpChanged;
};
