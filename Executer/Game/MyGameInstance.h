// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void SetMapBoss(const TObjectPtr<AActor> InMapBoss) { MapBoss = InMapBoss; }
	class AActor* GetMapBoss() const { return MapBoss; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> MapBoss;
};
