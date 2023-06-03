// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMontageManager.generated.h"

DECLARE_DELEGATE(FOnEndSkillSigniture)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UCharacterMontageManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterMontageManager();

// init section
public:
	void InitManager(class UAnimInstance* InAnimInstance, class UComboAttackDataAsset* InPlayerComboAttackData);

// animation section
public:
	void GetComboAttackCommand();

	void PlaySkillMontage(int32 MontageIndex);

	bool IsCoolTimeSkill(uint8 MontageIndex);

public:
	FOnEndSkillSigniture OnEndSkill;

// Animation Section
private:
	bool StopMontage();

	UFUNCTION()
	void EndAnimation(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetCanMove();

	void SetCoolTimeSkill(uint8 MontageIndex, bool InValue);

// Combo Animation Section
private:
	void ComboActionBegin();
	bool CheckDataValid();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();
	
// animation section
private:
	UPROPERTY()
	TObjectPtr<class UAnimInstance> PawnAnimInstance;

	UPROPERTY()
	TObjectPtr<class UComboAttackDataAsset> PlayerComboAttackData;

	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<class UCharacterSkillDataAsset> PlayerSkillData;

	TArray<bool> SkillCoolTimes;

	uint8 CurComboIndex = 0;
	uint8 CurMontageIndex = 0;
	FTimerHandle ComboTimerHandle;
	FTimerHandle SkillTimerHandle;
	uint8 bHasNextComboCommand : 1;
	uint8 bCanStop : 1;
};
