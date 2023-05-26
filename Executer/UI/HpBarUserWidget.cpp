// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HpBarUserWidget.h"
#include "Kismet/KismetMathLibrary.h"

UHpBarUserWidget::UHpBarUserWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TargetHp = -1.f;
	CurHp = -1.f;
	MaxHp = -1.f;
	InterpSpeed = 1.f;
	ConstInterpSpeed = 1.f;
}

void UHpBarUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CalcCurHpToTarget(InDeltaTime);
}

void UHpBarUserWidget::SetTargetHp(const float& InTargetHp)
{
	CurHp = TargetHp;
	TargetHp = InTargetHp;
}

void UHpBarUserWidget::CalcCurHpToTarget(const float& DeltaTime)
{
	if (CurHp == TargetHp)
	{
		return;
	}

	float InterpToHp = UKismetMathLibrary::FInterpTo(CurHp, TargetHp, DeltaTime, InterpSpeed);
	float InterpToConstHp = UKismetMathLibrary::FInterpTo_Constant(InterpToHp, TargetHp, DeltaTime, ConstInterpSpeed);

	CurHp = InterpToConstHp;

	float HpBarPercent = GetHpPercent();

	K2_OnChangeHpPercentCpp(HpBarPercent);
}
