// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacterSettingData.h"

UPlayerCharacterSettingData::UPlayerCharacterSettingData()
{
	MaxWalkSpeed = 700.f;
	JumpZVelocity = 600.f;
	AirControl = 0.5f;
	GravityScale = 2.f;

	TargetArmLength = 400.f;
}
