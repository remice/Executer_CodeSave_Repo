// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/MainPlayerController.h"
#include "Character/PlayerCharacterSettingData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Character/CharacterDodgeManager.h"
#include "Character/CharacterCameraManager.h"

/***********************
* Addresses of Asset
************************/
#define PATH_SK_KALLARI TEXT("/Game/ParagonKallari/Characters/Heroes/Kallari/Skins/Rogue/Meshes/Kallari_Rogue.Kallari_Rogue")
#define PATH_ABP_KALLARI TEXT("/Game/ParagonKallari/Characters/Heroes/Kallari/Kallari_AnimBlueprint.Kallari_AnimBlueprint")
#define PATH_CURVE_JUMP TEXT("/Game/Curve/JumpCurve.JumpCurve")
#define PATH_CURVE_DASH TEXT("/Game/Curve/DashCurve.DashCurve")
#define PATH_DASH_EFFECT TEXT("/Game/ImproveFights_Vfx/Particles/P_Darkness_Dash.P_Darkness_Dash")
#define PATH_DASHTRAIL_EFFECT TEXT("/Game/ImproveFights_Vfx/Particles/P_Darkness_DashTrail.P_Darkness_DashTrail")
#define PATH_SETTINGDATA TEXT("/Game/DataAssets/DA_BasicPlayer.DA_BasicPlayer")

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Spawn default objects
	CamArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamArm"));
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	DodgeManager = CreateDefaultSubobject<UCharacterDodgeManager>(TEXT("DodgeManager"));
	CameraManager = CreateDefaultSubobject<UCharacterCameraManager>(TEXT("CameraManager"));

	// Set collision size
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.0f);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	// Set rotation rule
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Set movement, rotation info
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->GravityScale = 2.f;
	// Rotate to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// Rotate speed rate
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Set character mesh transform
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	// Set spring arm info
	CamArm->SetupAttachment(RootComponent);
	CamArm->TargetArmLength = 400.f;
	CamArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	CamArm->bUsePawnControlRotation = true;

	// Attach camera to spring arm
	Cam->SetupAttachment(CamArm, USpringArmComponent::SocketName);
	Cam->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KALLARI(PATH_SK_KALLARI);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ABP_KALLARI(PATH_ABP_KALLARI);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CURVE_JUMP(PATH_CURVE_JUMP);
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CURVE_DASH(PATH_CURVE_DASH);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DASH_EFFECT(PATH_DASH_EFFECT);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> DASHTRAIL_EFFECT(PATH_DASHTRAIL_EFFECT);
	static ConstructorHelpers::FObjectFinder<UPlayerCharacterSettingData> SETTING_DATA(PATH_SETTINGDATA);

	check(SK_KALLARI.Succeeded());
	check(ABP_KALLARI.Succeeded());
	check(CURVE_JUMP.Succeeded());
	check(CURVE_DASH.Succeeded());
	check(DASH_EFFECT.Succeeded());
	check(DASHTRAIL_EFFECT.Succeeded());
	check(SETTING_DATA.Succeeded());

	// Apply find assets
	GetMesh()->SetSkeletalMesh(SK_KALLARI.Object);
	GetMesh()->SetAnimInstanceClass(ABP_KALLARI.Object->GeneratedClass);
	JumpCurve = CURVE_JUMP.Object;
	DashCurve = CURVE_DASH.Object;
	DashEffect = DASH_EFFECT.Object;
	DashTrailEffect = DASHTRAIL_EFFECT.Object;
	PlayerCharacterSettingData = SETTING_DATA.Object;

	// Set bone structs
	DashBoneSettings.Emplace(FDashBone(TEXT("HEAD"), 50.f, 3.f));
	DashBoneSettings.Emplace(FDashBone(TEXT("HAND_R"), 30.f, 4.f));
	DashBoneSettings.Emplace(FDashBone(TEXT("HAND_L"), 30.f, 4.f));
	DashBoneSettings.Emplace(FDashBone(TEXT("FOOT_R"), 30.f, 4.f));
	DashBoneSettings.Emplace(FDashBone(TEXT("FOOT_L"), 30.f, 4.f));

	// Init default value
	LongJumpTime = 0.75f;
	CurJumpTime = 0.f;
	MaxDashTime = 0.f;
	CurDashTime = 0.f;
	bOnDash = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupManagers();

	AExecuterPlayerState* ExecuterPlayerState = Cast<AExecuterPlayerState>(GetPlayerState());
	check(ExecuterPlayerState);

	ExecuterPlayerState->SetupHealth(1000);
	ExecuterPlayerState->SetupArmor(0);

	check(PlayerCharacterSettingData);
	SetCharacterSettingData(PlayerCharacterSettingData);

	SetTickEnable(false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DisableDodgeDelay -= DeltaTime;
	if (DisableDodgeDelay <= 0.f)
	{
		DodgeManager->SetTickEnable(true);
	}

	if (bOnDash)
	{
		ExDash(DeltaTime);
	}

	SetTickEnable(false);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get EIC & cast controller
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(Controller);
	check(EnhancedInputComponent && PlayerController);

	// Bind actions
	EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Ongoing, this, &APlayerCharacter::LongJump);
	EnhancedInputComponent->BindAction(PlayerController->JumpAction, ETriggerEvent::Canceled, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(PlayerController->DashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dash);
	EnhancedInputComponent->BindAction(PlayerController->CameraAutoAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CameraAutoPosMode);
	EnhancedInputComponent->BindAction(PlayerController->CameraFixedAction, ETriggerEvent::Triggered, this, &APlayerCharacter::CameraFixedMode);

	// Get local player
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	check(LocalPlayer);

	// Get enhanced subsystem
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// Add mapping context in subsystem
	Subsystem->AddMappingContext(PlayerController->PlayerMappingContext, 0);
}

void APlayerCharacter::Jump()
{
	Super::Jump();

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	JumpCurve->GetTimeRange(CurJumpTime, LongJumpTime);
}

void APlayerCharacter::StopJumping()
{
	Super::StopJumping();

	GetCharacterMovement()->GravityScale = 2.f;
	CurJumpTime = LongJumpTime;
}

void APlayerCharacter::DisableDodge(float DelayTime)
{
	DodgeManager->SetTickEnable(false);
	DisableDodgeDelay = DelayTime;
	SetTickEnable(true);
}

void APlayerCharacter::SetupManagers()
{
	// TODO : Use data asset
	CameraManager->InitManager(Cam, CamArm, GetController(), 0.5f);
	DodgeManager->InitManager();
}

void APlayerCharacter::SetCharacterSettingData(const UPlayerCharacterSettingData* Settingdata)
{
	// Set rotation rule
	bUseControllerRotationPitch = Settingdata->bUseControllerRotationPitch;
	bUseControllerRotationYaw = Settingdata->bUseControllerRotationYaw;
	bUseControllerRotationRoll = Settingdata->bUseControllerRotationRoll;

	// Set movement, rotation info
	GetCharacterMovement()->MaxWalkSpeed = Settingdata->MaxWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = Settingdata->JumpZVelocity;
	GetCharacterMovement()->AirControl = Settingdata->AirControl;
	GetCharacterMovement()->GravityScale = Settingdata->GravityScale;
	// Rotate to movement direction
	GetCharacterMovement()->bOrientRotationToMovement = Settingdata->bOrientRotationToMovement;
	// Rotate speed rate
	GetCharacterMovement()->RotationRate = Settingdata->RotationRate;

	// Set spring arm info
	CamArm->TargetArmLength = Settingdata->TargetArmLength;
	CamArm->SetRelativeRotation(Settingdata->RelativeRotation);
	CamArm->bUsePawnControlRotation = Settingdata->bUsePawnControlRotation;
}

void APlayerCharacter::AddProjectileIdsToSet(const TSet<int32> NearProjectileIds)
{
	DodgeManager->AddProjectileIdsToSet(NearProjectileIds);
}

void APlayerCharacter::SetTickEnable(bool IsOn)
{
	if (IsOn)
	{
		// tick enable
		PrimaryActorTick.SetTickFunctionEnable(IsOn);
		return;
	}

	if (DisableDodgeDelay > 0.f)
	{
		return;
	}

	if (bOnDash)
	{
		return;
	}
	// tick unable
	PrimaryActorTick.SetTickFunctionEnable(IsOn);
}

void APlayerCharacter::LongJump(const FInputActionValue& ActionValue)
{
	float DeltaTime = ActionValue.Get<float>();

	CurJumpTime += DeltaTime;

	float NewGravity = JumpCurve->GetFloatValue(CurJumpTime);
	
	GetCharacterMovement()->GravityScale = NewGravity;
}

void APlayerCharacter::ExDash(float DeltaTime)
{
	CurDashTime += DeltaTime;

	bool IsDashOver = CurDashTime > MaxDashTime;
	if (IsDashOver)
	{
		// Init dash(dash over)
		bOnDash = false;
		CurDashTime = 0.f;

		GetMesh()->SetVisibility(true);
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		// Spawn dash end effect
		UNiagaraFunctionLibrary::SpawnSystemAttached(DashEffect, GetMesh(), TEXT("NONE"), FVector(0.f), FRotator(0.f),
													 EAttachLocation::KeepRelativeOffset, true);
		// Deactivate all dash trail effect
		for (const auto DashEffectComponent : DashEffectComponents)
		{
			DashEffectComponent->Deactivate();
		}
		DashEffectComponents.Empty();

		return;
	}
	// Ongoing dash(Change XY-Plain velocity)
	float DashCurveValue = DashCurve->GetFloatValue(CurDashTime);
	FVector DashInputApplyCurve = LastInputVector * DashCurveValue * GetCharacterMovement()->MaxWalkSpeed;
	GetCharacterMovement()->Velocity.X = DashInputApplyCurve.X;
	GetCharacterMovement()->Velocity.Y = DashInputApplyCurve.Y;
}

void APlayerCharacter::Move(const FInputActionValue& ActionValue)
{
	if (bOnDash)
	{
		return;
	}

	FVector2D MoveDir = ActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get controller yaw rotation(not use roll, pitch value)
		const FRotator Rot = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rot.Yaw, 0);

		// Get unit axis vector from controller yaw rotation
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, MoveDir.Y);
		AddMovementInput(RightDir, MoveDir.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& ActionValue)
{
	FVector2D LookDir = ActionValue.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-LookDir.X);
		AddControllerPitchInput(LookDir.Y);
	}
}

void APlayerCharacter::Dash(const FInputActionValue& ActionValue)
{
	if (bOnDash == true)
	{
		return;
	}

	// Tick on
	SetTickEnable(true);

	// Init dash values
	bOnDash = true;
	DashCurve->GetTimeRange(CurDashTime, MaxDashTime);
	LastInputVector = GetCharacterMovement()->GetLastInputVector();
	LastInputVector.Normalize();

	if (IsValid(DashEffect) == false)
	{
		return;
	}

	if (IsValid(DashTrailEffect) == false)
	{
		return;
	}

	// Spawn start dash effect
	UNiagaraFunctionLibrary::SpawnSystemAttached(DashEffect, GetMesh(), TEXT("NONE"), FVector(0.f), FRotator(0.f),
												 EAttachLocation::KeepRelativeOffset, true);
	// Spawn dash trail effects
	for (const auto BoneSetting : DashBoneSettings)
	{
		UNiagaraComponent* DashTrailEffectComponent =
			UNiagaraFunctionLibrary::SpawnSystemAttached(DashTrailEffect, GetMesh(), BoneSetting.BoneName, FVector(0.f), FRotator(0.f),
														 EAttachLocation::KeepRelativeOffset, true);

		DashTrailEffectComponent->SetFloatParameter(TEXT("User.RibbonWidth"), BoneSetting.RibbonWidth);
		DashTrailEffectComponent->SetFloatParameter(TEXT("User.RibbonLenght"), BoneSetting.RibbonLength);
		DashTrailEffectComponent->SetFloatParameter(TEXT("User.Duration"), MaxDashTime);

		DashEffectComponents.Emplace(DashTrailEffectComponent);
	}
	
	GetMesh()->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
}

void APlayerCharacter::CameraAutoPosMode()
{
	CameraManager->OnAutoPositionMove();
}

void APlayerCharacter::CameraFixedMode()
{
	CameraManager->ToggleFixedMode();
}