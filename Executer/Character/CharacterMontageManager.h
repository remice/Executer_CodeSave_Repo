// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterMontageManager.generated.h"


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

	uint8 CurComboIndex = 0;
	FTimerHandle ComboTimerHandle;
	uint8 bHasNextComboCommand : 1;
};
