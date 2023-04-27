// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterMontageManager.h"
#include "Character/ComboAttackDataAsset.h"

// Sets default values for this component's properties
UCharacterMontageManager::UCharacterMontageManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bHasNextComboCommand = false;
}

void UCharacterMontageManager::InitManager(UAnimInstance* InAnimInstance, UComboAttackDataAsset* InPlayerComboAttackData)
{
	PawnAnimInstance = InAnimInstance;
	PlayerComboAttackData = InPlayerComboAttackData;
}

void UCharacterMontageManager::GetComboAttackCommand()
{
	if (CurComboIndex == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		bHasNextComboCommand = false;
	}
	else
	{
		bHasNextComboCommand = true;
	}
}

void UCharacterMontageManager::ComboActionBegin()
{
	if (CheckDataValid() == false)
	{
		return;
	}

	CurComboIndex = 1;

	const float AttackSpeedRate = 1.f;
	
	PawnAnimInstance->Montage_Play(PlayerComboAttackData->ComboAttackMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UCharacterMontageManager::ComboActionEnd);
	PawnAnimInstance->Montage_SetEndDelegate(EndDelegate, PlayerComboAttackData->ComboAttackMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

bool UCharacterMontageManager::CheckDataValid()
{
	bool bIsValidAllPtr = IsValid(PawnAnimInstance) && IsValid(PlayerComboAttackData);

	if (bIsValidAllPtr == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("montage manager has unvalid pointer"));
	}

	return bIsValidAllPtr;
}

void UCharacterMontageManager::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (CurComboIndex == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("montage manager cur combo count is 0!!"));
		return;
	}
	CurComboIndex = 0;
}

void UCharacterMontageManager::SetComboCheckTimer()
{
	int32 ComboIndex = CurComboIndex - 1;

	ensure(PlayerComboAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (PlayerComboAttackData->EffectiveFrameCount[ComboIndex] / PlayerComboAttackData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UCharacterMontageManager::ComboCheck, ComboEffectiveTime, false);
	}
}

void UCharacterMontageManager::ComboCheck()
{
	ComboTimerHandle.Invalidate();

	if (CheckDataValid() == false)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("1"));
	if (bHasNextComboCommand)
	{
		UE_LOG(LogTemp, Log, TEXT("2"));
		CurComboIndex = FMath::Clamp(CurComboIndex + 1, 1, PlayerComboAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *PlayerComboAttackData->MontageSectionNamePrefix, CurComboIndex);
		PawnAnimInstance->Montage_JumpToSection(NextSection, PlayerComboAttackData->ComboAttackMontage);
		SetComboCheckTimer();
		bHasNextComboCommand = false;
	}
}
