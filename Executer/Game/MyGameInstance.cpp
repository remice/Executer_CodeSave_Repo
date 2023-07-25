// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	PlayerStat.Hp = 1000;
	bTutoClear = false;
}

void UMyGameInstance::SetSaveStat(const FPlayerSaveStat& InSaveStat, ESaveMode SaveMode)
{
	FPlayerSaveStat ModifiedStat = PlayerStat;
	switch (SaveMode)
	{
	case ESaveMode::HpOnly:
		ModifiedStat.Hp = InSaveStat.Hp;
		break;

	case ESaveMode::GaugeOnly:
		ModifiedStat.SpecialGauge = InSaveStat.SpecialGauge;
		break;

	case ESaveMode::MoveTowardCameraOnly:
		ModifiedStat.bMoveTowardCameraOnly = InSaveStat.bMoveTowardCameraOnly;
		break;

	case ESaveMode::Every:
		ModifiedStat = InSaveStat;
		break;

	default:
		check(true);
	}
	PlayerStat = ModifiedStat;
}

bool UMyGameInstance::ToggleMoveTowardCamera()
{
	bool ToggleValue = PlayerStat.bMoveTowardCameraOnly;
	ToggleValue = !ToggleValue;
	PlayerStat.bMoveTowardCameraOnly = ToggleValue;
	return PlayerStat.bMoveTowardCameraOnly;
}