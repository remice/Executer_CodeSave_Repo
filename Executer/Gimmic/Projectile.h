// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class EXECUTER_API AProjectile : public AActor
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
	UPROPERTY()
	float Damage;

	UPROPERTY()
	int32 Id;

	UPROPERTY(VisibleAnywhere, Category=Mesh)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, Category=Component)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	virtual void OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FORCEINLINE void SetDamage(const float& InDamage) { Damage = InDamage; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE void SetId(const int32& InId) { Id = InId; }
	FORCEINLINE int32 GetId() const { return Id; }
	FORCEINLINE UStaticMeshComponent* GetBody() const { return Body; }
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
