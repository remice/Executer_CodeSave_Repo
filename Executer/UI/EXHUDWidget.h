// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EXHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UEXHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UEXHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) { bIsSucceedInteract = false; }

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnBossBar"))
	void K2_OnBossBar();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OffBossBar"))
	void K2_OffBossBar();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnQSkill"))
	void K2_OnQSkill(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnESkill"))
	void K2_OnESkill(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnRSkill"))
	void K2_OnRSkill(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnTSkill"))
	void K2_OnTSkill(float Cooldown);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnQSkillIconChanged"))
	void K2_OnQSkillIconChanged(const UTexture2D* NewIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnESkillIconChanged"))
	void K2_OnESkillIconChanged(const UTexture2D* NewIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnRSkillIconChanged"))
	void K2_OnRSkillIconChanged(const UTexture2D* NewIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnTSkillIconChanged"))
	void K2_OnTSkillIconChanged(const UTexture2D* NewIcon);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnOffInteractUI"))
	void K2_OnOffInteractUI(bool IsEnable);

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnInteractTextUI"))
	void K2_OnInteractTextUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "OnLevelEnd"))
	void K2_OnLevelEnd();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interact")
	uint8 bIsSucceedInteract : 1;
};
