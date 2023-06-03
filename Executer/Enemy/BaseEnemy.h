// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Interface/Initializable.h"
#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHpChangedSignature, float, CurrentHp, float, MaxHp);

UCLASS()
class EXECUTER_API ABaseEnemy : public APawn, public IInitializable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void Initialize() override;

protected:
	UPROPERTY(VisibleAnywhere, Category=Character)
	TObjectPtr<UCapsuleComponent> Collider;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float HP;

public:
	FORCEINLINE UCapsuleComponent* GetCollider() const { return Collider; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetHP() const { return HP; }
	void SetupHp(float InMaxHp);

	UPROPERTY(BlueprintAssignable, Category="Hp")
	FOnHpChangedSignature OnHpChanged;
};
