// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterCameraManager.generated.h"

UENUM()
enum class ECameraMode : uint8
{
	FixedMode = 1,
	AutoPosMode = 2,
	Default
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UCharacterCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterCameraManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

// Init section
public:
	UFUNCTION()
	void InitManager(class UCameraComponent* InCamera, class USpringArmComponent* InSpringArm, class AController* InController, const float InAutoPosTime = 0.5f) 
	{
		Camera = InCamera; SpringArm = InSpringArm; Controller = InController; AutoPosTime = InAutoPosTime;
	}

// Init Section
private:
	// Init camera manager(tick off)
	UFUNCTION()
	void InitAllSetting();

// Camera section
public:
	// Set camera mode to fixed mode
	UFUNCTION()
	void ToggleFixedMode();

	// Set camera mode to auto pos mode
	UFUNCTION()
	void OnAutoPositionMove();

// Camera section
private:
	// Calc target rotation to algorithm
	UFUNCTION()
	void CalcCameraSuitableRotator(const FVector& PlayerLoc, const FVector& BossLoc);

	// Update controller rotation to lerp
	UFUNCTION()
	void LerpRotateController(const FRotator& TargetRotation, const float LerpAlpha);

	// Excute camera fixed move (tick)
	UFUNCTION()
	void CameraFixedMove(float DeltaTime);

	// Execute camera auto position move (tick for AutoPosTime)
	UFUNCTION()
	void CameraAutoPositionMove();

// Camera section
private:
	UPROPERTY()
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY()
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY()
	TObjectPtr<class AController> Controller;

	UPROPERTY()
	TObjectPtr<class ABaseEnemy> Boss;

	UPROPERTY()
	FRotator TargetRot;

	ECameraMode CurMode;

private:
	float AutoPosTime;
	float CurTime;
	float DelayTime;
};
