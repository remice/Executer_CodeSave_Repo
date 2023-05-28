// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillSpawnUp.generated.h"

UCLASS()
class EXECUTER_API ASkillSpawnUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillSpawnUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void SpawnNiagaraEffect();
	virtual void MoveUpMesh();

protected:
	UPROPERTY(EditAnywhere, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> BlockerMesh;

	UPROPERTY()
	TObjectPtr<class USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraSystem> SpawnEffect;

protected:
	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	FVector MovePoint;

	UPROPERTY(EditAnywhere, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	float MoveTime;

	float LifeDuration;
	uint8 bOnMove : 1;
};
