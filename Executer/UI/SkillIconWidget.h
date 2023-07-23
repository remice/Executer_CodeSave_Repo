// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SkillIconWidget.generated.h"

USTRUCT()
struct FPercentModifier
{
	GENERATED_BODY()

public:
	FPercentModifier() : World(nullptr), Owner(nullptr) {}
	FPercentModifier(UWorld* InWorld, USkillIconWidget* InOwner) : World(InWorld), Owner(InOwner) {}

public:
	void CalcPercent();
	void SetPercent(float InTargetTime);

public:
	float CurTime;
	float TargetTime;
	float Percent;

	FTimerHandle TimerHandle;

	UPROPERTY()
	TWeakObjectPtr<UWorld> World;

	UPROPERTY()
	TWeakObjectPtr<USkillIconWidget> Owner;
};

/**
 * 
 */
UCLASS()
class EXECUTER_API USkillIconWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USkillIconWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnPercentChanged"))
	void K2_OnPercentChanged(float Percent);

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (DisplayName = "SetupPercent"))
	void SetupPercent(float Percent);

	UFUNCTION(BlueprintCallable, Category = "Custom", meta = (DisplayName = "DisableAllTimer"))
	void DisableAllTimer();

private:
	FPercentModifier PercentModifier;
};
