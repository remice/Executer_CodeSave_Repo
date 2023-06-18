// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FirstBossAnimInstance.h"
#include "FirstBoss.h"


UFirstBossAnimInstance::UFirstBossAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 100.f;
	bIsFalling = false;
	bIsJumping = false;
}

void UFirstBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AFirstBoss>(GetOwningActor());
}

void UFirstBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Owner)
	{
		Velocity = Owner->GetVelocity();
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Velocity.Z > JumpingThreshould || Velocity.Z < (-0.05) * JumpingThreshould;
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
		MoveDirection = Owner->GetGroundMoveRot();
	}
}