// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/ExecuterPlayerState.h"
#include "PlayerCharacter.generated.h"

USTRUCT()
struct FDashBone
{
	GENERATED_BODY()
	
	FDashBone() 
	{
		BoneName = TEXT("NONE");
		RibbonWidth = 0;
		RibbonLength = 0;
	}
	
	FDashBone(FName InBoneName, float InRibbonWidth, float InRibbonLength) : BoneName(InBoneName), RibbonWidth(InRibbonWidth), RibbonLength(InRibbonLength) {}
	
	UPROPERTY()
	FName BoneName;
	
	UPROPERTY()
	float RibbonWidth;

	UPROPERTY()
	float RibbonLength;
};

UCLASS()
class EXECUTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	virtual void StopJumping() override;

	void DisableDodge(float DelayTime);

// Setting section
protected:
	virtual void SetupManagers();

	virtual void SetCharacterSettingData(const class UPlayerCharacterSettingData* Settingdata);

// Dynamic dodge section
public:
	UFUNCTION()
	void AddProjectileIdsToSet(const TSet<int32> NearProjectileIds);

// Tick section
private:
	void SetTickEnable(bool IsOn);

// Move action section
private:
	UFUNCTION()
	void LongJump(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void ExDash(float DeltaTime);

	// input actions
	UFUNCTION()
	void Move(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void Look(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void Dash(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void CameraAutoPosMode();
	UFUNCTION()
	void CameraFixedMode();	

// manager section
private:
	UPROPERTY()
	TObjectPtr<class UCharacterDodgeManager> DodgeManager;
	UPROPERTY()
	TObjectPtr<class UCharacterCameraManager> CameraManager;

	float DisableDodgeDelay;

// component section
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> CamArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Cam;

private:
	// Jump property
	float LongJumpTime;
	float CurJumpTime;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCurveFloat> JumpCurve;

	// Dash property
	float MaxDashTime;
	float CurDashTime;
	uint8 bOnDash:1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCurveFloat> DashCurve;

	// Last move input vector(dash directional vector)
	FVector LastInputVector;

	UPROPERTY()
	TArray<FDashBone> DashBoneSettings;

	UPROPERTY(EditAnywhere, Category = Dash)
	TObjectPtr<class UNiagaraSystem> DashEffect;

	UPROPERTY(EditAnywhere, Category = Dash)
	TObjectPtr<class UNiagaraSystem> DashTrailEffect;

	UPROPERTY()
	TArray<TObjectPtr<class UNiagaraComponent>> DashEffectComponents;

	// Player default setting data
	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<class UPlayerCharacterSettingData> PlayerCharacterSettingData;
};