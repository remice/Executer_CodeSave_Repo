// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterMontageManager.h"
#include "Character/ComboAttackDataAsset.h"
#include "CharacterSkillDataAsset.h"
#include "Interface/CurveMovable.h"
#include "Curves/CurveVector.h"

// Sets default values for this component's properties
UCharacterMontageManager::UCharacterMontageManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bHasNextComboCommand = false;
	bCanStop = true;
}

void UCharacterMontageManager::InitManager(UAnimInstance* InAnimInstance, UComboAttackDataAsset* InPlayerComboAttackData)
{
	PawnAnimInstance = InAnimInstance;
	PlayerComboAttackData = InPlayerComboAttackData;
	if (PlayerSkillData.IsPending())
	{
		PlayerSkillData.LoadSynchronous();
	}

	for (const auto SkillDatas : PlayerSkillData.Get()->SkillAnimMontageDatas)
	{
		FSkillCoolTimeManager CoolTimeManager = FSkillCoolTimeManager(GetOwner()->GetWorld());
		CoolTimeManager.CoolTime = SkillDatas.CoolTime;
		SkillCoolTimes.Emplace(CoolTimeManager);
	}
}

void UCharacterMontageManager::GetComboAttackCommand()
{
	if (bCanStop == false)
	{
		return;
	}

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

const UTexture2D* UCharacterMontageManager::GetSkillIcon(uint8 MontageIndex)
{
	if (CheckDataValid() == false)
	{
		return nullptr;
	}

	int32 ValidMontageIndex = FMath::Clamp(MontageIndex, 0, PlayerSkillData->SkillAnimMontageDatas.Num() - 1);

	if (PlayerSkillData->SkillAnimMontageDatas[ValidMontageIndex].Icon.IsPending())
	{
		PlayerSkillData->SkillAnimMontageDatas[ValidMontageIndex].Icon.LoadSynchronous();
	}

	return PlayerSkillData->SkillAnimMontageDatas[ValidMontageIndex].Icon.Get();
}

bool UCharacterMontageManager::PlaySkillMontage(int32 MontageIndex, ESkillType SkillType)
{
	if (CheckDataValid() == false)
	{
		return false;
	}

	TArray<FSkillAnimMontageData> SkillMontages = PlayerSkillData->SkillAnimMontageDatas;
	if (SkillMontages.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Player Montage Manager] Skill anim montage data asset Montage is empty!!"));
		return false;
	}

	int32 ValidMontageIndex = FMath::Clamp(MontageIndex, 0, SkillMontages.Num() - 1);

	if (SkillMontages.IsValidIndex(ValidMontageIndex) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Player Montage Manager] Index %d SkillMontage doesn't exist!!"), ValidMontageIndex);
		return false;
	}

	UAnimMontage* ValidMontage = SkillMontages[ValidMontageIndex].Montage;
	if (IsValid(ValidMontage) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Player Montage Manager] Skill data asset Montage is empty!!"));
		return false;
	}

	if (bCanStop == false)
	{
		return false;
	}

	if (IsCoolTimeSkill(ValidMontageIndex)) return false;
	OnCoolTimeSkill(ValidMontageIndex, SkillType);

	StopMontage();

	CurComboIndex = 0;
	PawnAnimInstance->Montage_Play(ValidMontage);
	FOnMontageEnded OnMontageEnd;
	OnMontageEnd.BindUObject(this, &UCharacterMontageManager::EndAnimation);
	PawnAnimInstance->Montage_SetEndDelegate(OnMontageEnd);

	bCanStop = SkillMontages[ValidMontageIndex].bCanStop;
	if (bCanStop == false)
	{
		GetWorld()->GetTimerManager().SetTimer(SkillTimerHandle, this, &UCharacterMontageManager::SetCanMove, SkillMontages[ValidMontageIndex].StopTime, false);
	}

	if (SkillMontages[ValidMontageIndex].bHasCurve)
	{
		ICurveMovable* OwnerPawn = Cast<ICurveMovable>(GetOwner());
		if (OwnerPawn)
		{
			OwnerPawn->StartCurveMove(SkillMontages[ValidMontageIndex].CurveData, true, true);
		}
	}

	return true;
}

bool UCharacterMontageManager::IsCoolTimeSkill(uint8 MontageIndex)
{
	uint8 ModifyIndex = FMath::Clamp(MontageIndex, 0, SkillCoolTimes.Num() - 1);

	return SkillCoolTimes[ModifyIndex].bOnCool;
}

void UCharacterMontageManager::DisableAllTimer()
{
	for (auto CooldownManager : SkillCoolTimes)
	{
		CooldownManager.StopTimer();
	}
}

bool UCharacterMontageManager::StopMontage()
{
	if (bCanStop == false)
	{
		return false;
	}

	if (ComboTimerHandle.IsValid())
	{
		ComboTimerHandle.Invalidate();
		CurComboIndex = 0;
	}

	if (SkillTimerHandle.IsValid())
	{
		SkillTimerHandle.Invalidate();
	}

	PawnAnimInstance->Montage_Stop(0.f);
	return true;
}

void UCharacterMontageManager::EndAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	OnEndSkill.ExecuteIfBound();
}

void UCharacterMontageManager::SetCanMove()
{
	CurComboIndex = 0;
	bCanStop = true;
}

void UCharacterMontageManager::OnCoolTimeSkill(uint8 MontageIndex, ESkillType SkillType)
{
	uint8 ModifyIndex = FMath::Clamp(MontageIndex, 0, SkillCoolTimes.Num() - 1);

	SkillCoolTimes[ModifyIndex].ExecuteCooldown();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	IExecuterControllerInterface* UIController = Cast<IExecuterControllerInterface>(OwnerPawn ? OwnerPawn->GetController() : nullptr);
	if (UIController)
	{
		UIController->OnSkillCooldownUI(SkillType, SkillCoolTimes[ModifyIndex].CoolTime);
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

	if (PlayerSkillData.IsPending())
	{
		PlayerSkillData.LoadSynchronous();
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

	if (bHasNextComboCommand)
	{
		CurComboIndex = FMath::Clamp(CurComboIndex + 1, 1, PlayerComboAttackData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *PlayerComboAttackData->MontageSectionNamePrefix, CurComboIndex);
		PawnAnimInstance->Montage_JumpToSection(NextSection, PlayerComboAttackData->ComboAttackMontage);
		SetComboCheckTimer();
		bHasNextComboCommand = false;
	}
}

// Struct Section

void FSkillCoolTimeManager::ExecuteCooldown()
{
	if (World.IsValid() == false)
	{
		return;
	}

	if (bOnCool == true)
	{
		return;
	}

	bOnCool = true;

	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[&] {
			if (World.IsValid() == false) return;
			bOnCool = false;
		});
	World.Get()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, CoolTime, false);
}

void FSkillCoolTimeManager::StopTimer()
{
	if (World.IsValid())
	{
		World.Get()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
