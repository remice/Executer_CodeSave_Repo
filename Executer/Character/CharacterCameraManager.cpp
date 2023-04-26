// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterCameraManager.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Game/MyGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UCharacterCameraManager::UCharacterCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Init values
	CurMode = ECameraMode::Default;

	AutoPosTime = 0.5f;
	CurTime = 0.f;
	DelayTime = 0.f;
}


// Called when the game starts
void UCharacterCameraManager::BeginPlay()
{
	Super::BeginPlay();

	// Unable tick
	PrimaryComponentTick.SetTickFunctionEnable(false);

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetOwner()->GetGameInstance());
	if (GI)
	{
		Boss = GI->GetMapBoss();
	}
	
}


// Called every frame
void UCharacterCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If max delay, unable manager & init
	if (CurTime > DelayTime)
	{
		InitAllSetting();
		return;
	}
	CurTime += DeltaTime;

	// Update camera rotation
	switch (CurMode)
	{
	case ECameraMode::FixedMode:
		CameraFixedMove(DeltaTime);
		break;

	case ECameraMode::AutoPosMode:
		CameraAutoPositionMove();
		break;

	case ECameraMode::Default:
		// Do nothing
		break;
	// error
	default:
		UE_LOG(LogTemp, Error, TEXT("Cur camera mode setting ??? error [CharacterCameraManager/Tick]"));
		return;
	}
}

// Init camera manager(tick off)
void UCharacterCameraManager::InitAllSetting()
{
	PrimaryComponentTick.SetTickFunctionEnable(false);

	CurMode = ECameraMode::Default;
	CurTime = 0.f;
	DelayTime = 0.f;
	TargetRot = FRotator();
}

void UCharacterCameraManager::ToggleFixedMode()
{
	if (IsValid(Boss) == false)
	{
		return;
	}

	// toggle
	if (CurMode == ECameraMode::FixedMode)
	{
		InitAllSetting();
		return;
	}

	InitAllSetting();

	CurMode = ECameraMode::FixedMode;
	DelayTime = 9999999;

	// tick enable
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

void UCharacterCameraManager::OnAutoPositionMove()
{
	// if setting time is 0, do nothing
	if (AutoPosTime <= 0)
	{
		return;
	}
	
	if (IsValid(Boss) == false)
	{
		return;
	}

	InitAllSetting();

	CurMode = ECameraMode::AutoPosMode;
	DelayTime = AutoPosTime;
	// Calc init target rotation
	CalcCameraSuitableRotator(GetOwner()->GetActorLocation(), Boss->GetActorLocation());

	// tick enable
	PrimaryComponentTick.SetTickFunctionEnable(true);
}

// Calc target rotation to algorithm
void UCharacterCameraManager::CalcCameraSuitableRotator(const FVector& PlayerLoc, const FVector& BossLoc)
{
	// TODO Make algorithm camera best position
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLoc, BossLoc);
	LookRotation.Add(-20.f, 0.f, 0.f);

	TargetRot = LookRotation;
}

// Update controller rotation to lerp
void UCharacterCameraManager::LerpRotateController(const FRotator& TargetRotation, const float LerpAlpha)
{
	Controller->SetControlRotation(FMath::Lerp(Controller->GetControlRotation(), TargetRotation, LerpAlpha));
}

// Excute camera fixed move (tick)
void UCharacterCameraManager::CameraFixedMove(float DeltaTime)
{
	// If manager's not init, return
	bool OnInit = IsValid(Camera) && IsValid(SpringArm) && IsValid(Boss) && IsValid(Controller);
	if (OnInit == false)
	{
		return;
	}

	// Calc TargetRot for every tick
	CalcCameraSuitableRotator(GetOwner()->GetActorLocation(), Boss->GetActorLocation());

	LerpRotateController(TargetRot, 5 * DeltaTime);
}

// Execute camera auto position move (tick for AutoPosTime)
void UCharacterCameraManager::CameraAutoPositionMove()
{
	// If manager's not init, return
	bool OnInit = IsValid(Camera) && IsValid(SpringArm) && IsValid(Boss) && IsValid(Controller);
	if (OnInit == false)
	{
		return;
	}

	// Devide 0 error exception
	if (DelayTime == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Devide 0 error [CharacterCameraManager/CameraAutoPositionMove]"));
		return;
	}
	float LerpAlpha = CurTime / DelayTime;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	LerpRotateController(TargetRot, LerpAlpha);
}

