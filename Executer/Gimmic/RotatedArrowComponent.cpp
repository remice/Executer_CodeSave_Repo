// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatedArrowComponent.h"

URotatedArrowComponent::URotatedArrowComponent()
{
	RotateOption = ERotateOption::Sin;
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
	float NewYaw = 0;

	switch (RotateOption)
	{
	case ERotateOption::Sin:
		NewYaw = sin(SpanTime * RotSpeed) * RotAngle;
		break;

	case ERotateOption::Linear:
		NewYaw = (SpanTime * RotSpeed) * RotAngle;
		break;

	case ERotateOption::Default:
		NewYaw = 0;
	}

	NewRot.Add(0.f, NewYaw, 0.f);
	NewRot.Clamp();

	SetRelativeRotation(NewRot);
}
