// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player/ExecuterPlayerState.h"
#include "Interface/CanDodgeActor.h"
#include "Interface/CurveMovable.h"
#include "Interface/AttackCheckable.h"
#include "Interface/Initializable.h"
#include "PlayerCharacter.generated.h"

USTRUCT()
struct FDashBone
{
	GENERATED_BODY()
	
	FDashBone() 
	{
		BoneName = TEXT("NONE");
		RibbonWidth = 0;
		RibbonLength = 0;
	}
	
	FDashBone(FName InBoneName, float InRibbonWidth, float InRibbonLength) : BoneName(InBoneName), RibbonWidth(InRibbonWidth), RibbonLength(InRibbonLength) {}
	
	UPROPERTY()
	FName BoneName;
	
	UPROPERTY()
	float RibbonWidth;

	UPROPERTY()
	float RibbonLength;
};

UCLASS()
class EXECUTER_API APlayerCharacter : public ACharacter, public ICanDodgeActor, public ICurveMovable, public IAttackCheckable, public IInitializable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Initialize() override;

	virtual void Jump() override;
	virtual void StopJumping() override;

// Hit Section
protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Setting section
protected:
	virtual void SetupManagers();

	virtual void SetCharacterSettingData(const class UPlayerCharacterSettingData* Settingdata);

// Dynamic dodge section
public:
	UFUNCTION()
	virtual void AddProjectileIdsToSet(const TSet<int32> NearProjectileIds) override;
	virtual void AddProjectileIdToSet(const int32& NearProjectileId) override;

// Curve movement section
public:
	virtual void StartCurveMove(class UCurveVector* CurveData, bool LockPlayerMove, bool LookControlRot) override;

// Attack section
public:
	virtual bool CheckAttachToSocket(const FName& SocketName, const FVector& PreFrameLocation, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray, TArray<TObjectPtr<AActor>> IgnoreActorArray, FHitResult& HitResult) override;
	virtual FVector GetLocationToSocket(const FName& SocketName) override;

// Tick section
private:
	void SetTickEnable(bool IsOn);

// Move action section
private:
	UFUNCTION()
	void LongJump(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void ExDash(float DeltaTime);

	UFUNCTION()
	void ExCurveMove(float DeltaTime);

	void EndCurveMove();

// input actions
private:
	UFUNCTION()
	void Move(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void Look(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void Dash(const struct FInputActionValue& ActionValue);

	UFUNCTION()
	void CameraAutoPosMode();
	UFUNCTION()
	void CameraFixedMode();

	UFUNCTION()
	void QERTSkill(const struct FInputActionValue& ActionValue);

// manager section
private:
	UPROPERTY()
	TObjectPtr<class UCharacterDodgeManager> DodgeManager;
	UPROPERTY()
	TObjectPtr<class UCharacterCameraManager> CameraManager;
	UPROPERTY()
	TObjectPtr<class UCharacterMontageManager> MontageManager;

	float DisableDodgeDelay = 1.f;

// component section
private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class USpringArmComponent> CamArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<class UCameraComponent> Cam;

private:
	// Jump property
	float LongJumpTime;
	float CurJumpTime;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCurveFloat> JumpCurve;

	// Dash property
	float MaxDashTime;
	float CurDashTime;
	uint8 bOnDash:1;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCurveFloat> DashCurve;

	// Last move input vector(dash directional vector)
	FVector LastInputVector;

	UPROPERTY()
	TArray<FDashBone> DashBoneSettings;

	UPROPERTY(EditAnywhere, Category = Dash)
	TObjectPtr<class UNiagaraSystem> DashEffect;

	UPROPERTY(EditAnywhere, Category = Dash)
	TObjectPtr<class UNiagaraSystem> DashTrailEffect;

	UPROPERTY()
	TArray<TObjectPtr<class UNiagaraComponent>> DashEffectComponents;

	// Player default setting data
	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<class UPlayerCharacterSettingData> PlayerCharacterSettingData;

	// Skill Section
	UPROPERTY(EditAnywhere, Category = Skill)
	int32 SkillQMontageIndex;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 SkillEMontageIndex;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 SkillRMontageIndex;

	UPROPERTY(EditAnywhere, Category = Skill)
	int32 SkillTMontageIndex;

// animation section
private:
	UPROPERTY(EditAnywhere, Category = Data)
	TObjectPtr<class UComboAttackDataAsset> PlayerComboAttackData;

	void ComboAttack();

// Curve Movement Section
private:
	UPROPERTY()
	TObjectPtr<class UCurveVector> MoveCurveVector;

	float MaxCurveMoveTime = 1.f;
	float CurCurveMoveTime = 1.f;
	uint8 bOnCurveMove : 1;
	uint8 bLockMove : 1;
};
