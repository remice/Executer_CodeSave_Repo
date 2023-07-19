// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExecuterGIInterface.generated.h"

UENUM()
enum class ESaveMode
{
	Every = 0,
	HpOnly,
	GaugeOnly
};

USTRUCT()
struct FPlayerSaveStat
{
	GENERATED_BODY()
	
public:
	FPlayerSaveStat() {}

public:
	float Hp = 0;
	float SpecialGauge = 0;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExecuterGIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXECUTER_API IExecuterGIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetMapBoss(AActor* InMapBoss) {}
	virtual AActor* GetMapBoss() const { return nullptr; }
	virtual const FPlayerSaveStat& GetSaveStat() const { return DefaultStat; }
	virtual void SetSaveStat(const FPlayerSaveStat& InSaveStat, ESaveMode SaveMode = ESaveMode::Every) {}

private:
	FPlayerSaveStat DefaultStat = FPlayerSaveStat();
};
