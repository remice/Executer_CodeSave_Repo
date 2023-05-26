// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXECUTER_API UHpBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHpBarUserWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Hp", meta = (DisplayName = "GetHpPercent"))
	FORCEINLINE float GetHpPercent() const { return (MaxHp == 0) ? 1.f : CurHp / MaxHp; }

	UFUNCTION(BlueprintCallable, Category = "Hp", meta = (DisplayName = "SetTargetHp"))
	void SetTargetHp(const float& InTargetHp);

	UFUNCTION(BlueprintCallable, Category = "Hp", meta = (DisplayName = "SetMaxHp"))
	FORCEINLINE void SetMaxHp(const float& InMaxHp) { MaxHp = InMaxHp; }

protected:
	void CalcCurHpToTarget(const float& DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, Category = "Hp", meta = (DisplayName = "OnChangeHpPercent"))
	void K2_OnChangeHpPercentCpp(const float& HpBarPercent);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (AllowPrivateAccess = "true"))
	float ConstInterpSpeed;

	float TargetHp;
	float CurHp;
	float MaxHp;

};
