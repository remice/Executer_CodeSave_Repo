// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Interface/IRotatableObject.h"
#include "RotatedArrowComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = Utility, editinlinenew, meta = (BlueprintSpawnableComponent))
class EXECUTER_API URotatedArrowComponent : public UArrowComponent, public IIRotatableObject
{
	GENERATED_BODY()
	
public:
	URotatedArrowComponent();

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void RelativeRotate(float SpanTime) override;

protected:
	UPROPERTY()
	FRotator3d InitRot;

	UPROPERTY(EditAnywhere, Category = Rotation)
	ERotateOption RotateOption;

	UPROPERTY(EditAnywhere, Category = Rotation)
	ERotateDirection RotateDirection;

	UPROPERTY(EditAnywhere, Category = Rotation)
	float RotAngle;

	UPROPERTY(EditAnywhere, Category = Rotation)
	float RotSpeed;
};
