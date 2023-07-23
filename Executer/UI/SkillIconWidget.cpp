// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillIconWidget.h"

USkillIconWidget::USkillIconWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USkillIconWidget::SetupPercent(float Percent)
{
	PercentModifier.SetPercent(Percent);
}

void USkillIconWidget::DisableAllTimer()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(PercentModifier.TimerHandle);
	}
}

void USkillIconWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PercentModifier = FPercentModifier(GetWorld(), this);
}

void FPercentModifier::CalcPercent()
{
	if (World.IsValid() == false)
	{
		return;
	}

	CurTime += World->GetTimerManager().GetTimerRate(TimerHandle);

	if (TargetTime <= 0)
	{
		Percent = 0;
		return;
	}
	else if (CurTime >= TargetTime || CurTime < 0)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	float CalcPercent = CurTime / TargetTime;
	Percent = FMath::Clamp(CalcPercent, 0.f, 1.f);
	Owner->K2_OnPercentChanged(Percent);
}

void FPercentModifier::SetPercent(float InTargetTime)
{
	if (Owner.IsValid() == false) return;

	CurTime = 0.f;
	TargetTime = InTargetTime;
	Percent = 0.f;

	if (World.IsValid() == false)
	{
		return;
	}

	if (TimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
	}

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[&]() {
			CalcPercent();
		});

	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, World->GetDeltaSeconds(), true);
}
