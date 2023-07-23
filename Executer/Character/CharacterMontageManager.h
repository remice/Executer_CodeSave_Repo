// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/ExecuterControllerInterface.h"

#include "CharacterMontageManager.generated.h"

DECLARE_DELEGATE(FOnEndSkillCooltimeSignature)

USTRUCT()
struct FSkillCoolTimeManager
{
	GENERATED_BODY()
	
public:
	FSkillCoolTimeManager() : bOnCool(false), World(nullptr) {}
	FSkillCoolTimeManager(UWorld* InWorld) : bOnCool(false), World(InWorld) {};
	~FSkillCoolTimeManager() { World.Reset(); }

public:
	void ExecuteCooldown();
	void StopTimer();

	float CoolTime = 0.1f;
	uint8 bOnCool : 1;

	UPROPERTY()
	TWeakObjectPtr<UWorld> World;

	FTimerHandle TimerHandle;
};

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
	const UTexture2D* GetSkillIcon(uint8 MontageIndex);
	bool PlaySkillMontage(int32 MontageIndex, ESkillType SkillType);
	bool IsCoolTimeSkill(uint8 MontageIndex); 
	void DisableAllTimer();

public:
	FOnEndSkillSigniture OnEndSkill;

// Animation Section
private:
	bool StopMontage();

	UFUNCTION()
	void EndAnimation(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void SetCanMove();

	void OnCoolTimeSkill(uint8 MontageIndex, ESkillType SkillType);

// Combo Animation Section
private:
	void ComboActionBegin();
	bool CheckDataValid();
	UFUNCTION()
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
	TSoftObjectPtr<class UCharacterSkillDataAsset> PlayerSkillData;

	TArray<FSkillCoolTimeManager> SkillCoolTimes;

	uint8 CurComboIndex = 0;
	uint8 CurMontageIndex = 0;
	FTimerHandle ComboTimerHandle;
	FTimerHandle SkillTimerHandle;
	uint8 bHasNextComboCommand : 1;
	uint8 bCanStop : 1;
};
