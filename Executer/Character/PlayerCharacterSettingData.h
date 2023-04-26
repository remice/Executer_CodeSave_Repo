// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerCharacterSettingData.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UPlayerCharacterSettingData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPlayerCharacterSettingData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationPitch : 1;

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationYaw : 1;

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint8 bUseControllerRotationRoll : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint8 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float JumpZVelocity;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float AirControl;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	float GravityScale;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint8 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;
	
};
