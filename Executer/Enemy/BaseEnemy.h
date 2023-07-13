// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/Initializable.h"
#include "Components/CapsuleComponent.h"

#include "BaseEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHpChangedSignature, float, CurrentHp, float, MaxHp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathedSignature);

UCLASS()
class EXECUTER_API ABaseEnemy : public APawn, public IInitializable
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseEnemy();

public:	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void CallInitialize() override;

protected:
	virtual void OnDeath();

protected:
	UPROPERTY(VisibleAnywhere, Category = Character)
	TObjectPtr<UCapsuleComponent> Collider;

	UPROPERTY(EditAnywhere, Category = Animation)
	TObjectPtr<class UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category=Info)
	float HP;

	UPROPERTY(EditAnywhere, Category = Animation)
	float AfterDeathTime;

	uint8 bIsDead;

private:
	UPROPERTY(EditAnywhere, Category=Info)
	float InitialHP;

public:
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return Collider; }
	FORCEINLINE float GetMaxHP() const { return MaxHP; }
	FORCEINLINE float GetHP() const { return HP; }
	void SetupHp(float InMaxHp);

	UPROPERTY(BlueprintAssignable, Category="Delegate")
	FOnHpChangedSignature OnHpChanged;

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FOnDeathedSignature OnDeathed;
};
