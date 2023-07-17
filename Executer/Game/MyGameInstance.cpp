// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

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

	case ESaveMode::Every:
		ModifiedStat = InSaveStat;
		break;

	default:
		check(true);
	}
	PlayerStat = ModifiedStat;
}
