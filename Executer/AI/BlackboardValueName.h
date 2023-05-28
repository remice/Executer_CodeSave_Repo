
#pragma once
#include "CoreMinimal.h"

#define BBPROPERTY_PlayerPawn TEXT("PlayerPawn")
#define BBPROPERTY_DistanceState TEXT("DistanceState")

UENUM(BlueprintType)
enum class EDistanceState : uint8
{
	Near =		0	UMETA(DisplayName = "Near"),
	Middle =	1	UMETA(DisplayName = "Middle"),
	Far	=		2	UMETA(DisplayName = "Far")
};