// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/CanBeDodgedActor.h"
#include "Projectile.generated.h"

UCLASS()
class EXECUTER_API AProjectile : public AAttackBase, public ICanBeDodgedActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = Effect)
	TObjectPtr<class UNiagaraSystem> HitEffect;

	UPROPERTY(VisibleAnywhere, Category=Mesh)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category=Component)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	virtual void OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual int32 GetId() override;

	FORCEINLINE UStaticMeshComponent* GetBody() const { return Body; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	virtual void SpawnHitEffect();
};
