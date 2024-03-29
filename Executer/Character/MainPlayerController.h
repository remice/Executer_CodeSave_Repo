// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interface/ExecuterControllerInterface.h"

#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API AMainPlayerController : public APlayerController, public IExecuterControllerInterface
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;

public:
	virtual void OnBossHpBar() override;
	virtual void OnSkillCooldownUI(ESkillType SkillType, float Time) override;
	virtual void OnChangedSkillUI(ESkillType SkillType, const UTexture2D* Icon) override;
	UFUNCTION()
	void OnOffInteractUI(bool IsEnable);
	bool InteractTextUI();
	void OnLevelEnd();
	void OnWidget(UUserWidget* Widget);

private:
	void InitHUDWidget();

// Enhanced Input Section
public:
	UPROPERTY(VisibleAnywhere, Category=Input)
	TObjectPtr<class UInputMappingContext> PlayerMappingContext;

	UPROPERTY(VisibleAnywhere, Category=Input)
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category=Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> DashAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> CameraAutoAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> CameraFixedAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ComboAttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SkillAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> SpecialAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<class UInputAction> OptionAction;

// UI Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UEXHUDWidget> HudWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<class UEXHUDWidget> HudWidget;
};
