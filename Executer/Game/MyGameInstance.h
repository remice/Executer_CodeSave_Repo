// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/ExecuterGIInterface.h"

#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UMyGameInstance : public UGameInstance, public IExecuterGIInterface
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	
public:
	// implement ExecuterGI interface
	virtual void SetMapBoss(AActor* InMapBoss) override { MapBoss = InMapBoss; }
	virtual AActor* GetMapBoss() const override { return MapBoss; }
	virtual void SetSaveStat(const FPlayerSaveStat& InSaveStat, ESaveMode SaveMode = ESaveMode::Every) override;
	virtual const FPlayerSaveStat& GetSaveStat() const override { return PlayerStat; }
	// end implement

	UFUNCTION(BlueprintCallable, Category="ClearFlag")
	void TutorialMapClear() {bTutoClear = true;}
	UFUNCTION(BlueprintCallable, Category = "Custom")
	bool ToggleMoveTowardCamera();
	UFUNCTION(BlueprintCallable, Category = "Custom")
	bool GetMoveTowardCamera() { return PlayerStat.bMoveTowardCameraOnly; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boss", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> MapBoss;

	FPlayerSaveStat PlayerStat;

	UPROPERTY(BlueprintReadOnly, Category="ClearFlag", meta = (AllowPrivateAccess = "true"))
	uint8 bTutoClear : 1;
};
