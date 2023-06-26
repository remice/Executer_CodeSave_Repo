// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatedArrowComponent.h"

URotatedArrowComponent::URotatedArrowComponent()
{
	RotateOption = ERotateOption::Sin;
	RotateDirection = ERotateDirection::Yaw;
	RotAngle = 90.f;
	RotSpeed = 1.f;
}

void URotatedArrowComponent::BeginPlay()
{
	Super::BeginPlay();

	InitRot = GetRelativeRotation();
}

void URotatedArrowComponent::RelativeRotate(float SpanTime)
{
	FRotator3d NewRot = InitRot;
	float NewValue = 0;

	switch (RotateOption)
	{
	case ERotateOption::Sin:
		NewValue = sin(SpanTime * RotSpeed) * RotAngle;
		break;

	case ERotateOption::Linear:
		NewValue = (SpanTime * RotSpeed) * RotAngle;
		break;

	case ERotateOption::Default:
		NewValue = 0;
	}

	switch (RotateDirection)
	{
	case ERotateDirection::Roll:
		NewRot.Add(0.f, 0.f, NewValue);
		break;
	case ERotateDirection::Yaw:
		NewRot.Add(0.f, NewValue, 0.f);
		break;
	case ERotateDirection::Pitch:
		NewRot.Add(NewValue, 0.f, 0.f);
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("[RotatedArrowComponent] ERotateDirection is unknown value!!"));
		break;
	}
	NewRot.Clamp();

	SetRelativeRotation(NewRot);
}
