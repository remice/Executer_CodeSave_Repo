// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "UI/EXHUDWidget.h"
#include "Interface/Initializable.h"
#include "Game/MyGameInstance.h"
#include "GameFramework/PlayerState.h"


#define PATH_IMC TEXT("/Game/Input/IMC_PlayerDefault.IMC_PlayerDefault")
#define PATH_IA_LOOK TEXT("/Game/Input/Action/IA_Look.IA_Look")
#define PATH_IA_MOVE TEXT("/Game/Input/Action/IA_Move.IA_Move")
#define PATH_IA_JUMP TEXT("/Game/Input/Action/IA_Jump.IA_Jump")
#define PATH_IA_DASH TEXT("/Game/Input/Action/IA_Dash.IA_Dash")
#define PATH_IA_CAMERAAUTO TEXT("/Game/Input/Action/IA_CameraAuto.IA_CameraAuto")
#define PATH_IA_CAMERAFIXED TEXT("/Game/Input/Action/IA_CameraFixed.IA_CameraFixed")
#define PATH_IA_COMBOATTACK TEXT("/Game/Input/Action/IA_ComboAttack.IA_ComboAttack")
#define PATH_IA_SKILL TEXT("/Game/Input/Action/IA_Skill.IA_Skill")
#define PATH_IA_SPECIAL TEXT("/Game/Input/Action/IA_Special.IA_Special")
#define PATH_IA_INTERACT TEXT("/Game/Input/Action/IA_Interact.IA_Interact")
#define PATH_HUDWIDGET_C TEXT("/Game/UI/WBP_EXHUD.WBP_EXHUD_C")

AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(PATH_IMC);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOOK(PATH_IA_LOOK);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(PATH_IA_MOVE);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JUMP(PATH_IA_JUMP);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DASH(PATH_IA_DASH);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CAMERAAUTO(PATH_IA_CAMERAAUTO);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CAMERAFIXED(PATH_IA_CAMERAFIXED);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_COMBOATTACK(PATH_IA_COMBOATTACK);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SKILL(PATH_IA_SKILL);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SPECIAL(PATH_IA_SPECIAL);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_INTERACT(PATH_IA_INTERACT);
	static ConstructorHelpers::FClassFinder<UEXHUDWidget> HUDWIDGET_C(PATH_HUDWIDGET_C);

	check(IMC.Succeeded());
	check(IA_LOOK.Succeeded());
	check(IA_MOVE.Succeeded());
	check(IA_JUMP.Succeeded());
	check(IA_DASH.Succeeded());
	check(IA_CAMERAAUTO.Succeeded());
	check(IA_CAMERAFIXED.Succeeded());
	check(IA_COMBOATTACK.Succeeded());
	check(IA_SKILL.Succeeded());
	check(IA_SPECIAL.Succeeded());
	check(IA_INTERACT.Succeeded());
	check(HUDWIDGET_C.Succeeded());

	PlayerMappingContext = IMC.Object;
	LookAction = IA_LOOK.Object;
	MoveAction = IA_MOVE.Object;
	JumpAction = IA_JUMP.Object;
	DashAction = IA_DASH.Object;
	CameraAutoAction = IA_CAMERAAUTO.Object;
	CameraFixedAction = IA_CAMERAFIXED.Object;
	ComboAttackAction = IA_COMBOATTACK.Object;
	SkillAction = IA_SKILL.Object;
	SpecialAction = IA_SPECIAL.Object;
	InteractAction = IA_INTERACT.Object;
	HudWidgetClass = HUDWIDGET_C.Class;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	InitHUDWidget();
}

void AMainPlayerController::OnBossHpBar()
{
	HudWidget->K2_OnBossBar();

	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (IsValid(GI) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] GameInstance are not exist!!"));
		return;
	}

	IInitializable* PreInitActor = Cast<IInitializable>(GI->GetMapBoss());
	if (PreInitActor)
	{
		PreInitActor->CallInitialize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] MapBoss are not exist!!"));
	}
}

void AMainPlayerController::OnSkillCooldownUI(ESkillType SkillType, float Time)
{
	switch (SkillType)
	{
	case ESkillType::Q:
		HudWidget->K2_OnQSkill(Time);
		break;

	case ESkillType::E:
		HudWidget->K2_OnESkill(Time);
		break;

	case ESkillType::R:
		HudWidget->K2_OnRSkill(Time);
		break;

	case ESkillType::T:
		HudWidget->K2_OnTSkill(Time);
		break;

	case ESkillType::Special:
		break;
	}
}

void AMainPlayerController::OnChangedSkillUI(ESkillType SkillType, const UTexture2D* Icon)
{
	switch (SkillType)
	{
	case ESkillType::Q:
		HudWidget->K2_OnQSkillIconChanged(Icon);
		break;

	case ESkillType::E:
		HudWidget->K2_OnESkillIconChanged(Icon);
		break;

	case ESkillType::R:
		HudWidget->K2_OnRSkillIconChanged(Icon);
		break;

	case ESkillType::T:
		HudWidget->K2_OnTSkillIconChanged(Icon);
		break;

	case ESkillType::Special:
		break;
	}
}

void AMainPlayerController::OnOffInteractUI(bool IsEnable)
{
	HudWidget->K2_OnOffInteractUI(IsEnable);
}

bool AMainPlayerController::InteractTextUI()
{
	HudWidget->K2_OnInteractTextUI();
	bool bResult = HudWidget->bIsSucceedInteract;
	return bResult;
}

void AMainPlayerController::OnLevelEnd()
{
	HudWidget->K2_OnLevelEnd();
}

void AMainPlayerController::InitHUDWidget()
{
	HudWidget = CreateWidget<UEXHUDWidget>(this, HudWidgetClass);
	if (IsValid(HudWidget) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] HUD are not create!!"));
		return;
	}

	UMyGameInstance* GI = GetGameInstance<UMyGameInstance>();
	if (IsValid(GI) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] GameInstance are not exist!!"));
		return;
	}

	HudWidget->AddToViewport();

	IInitializable* PostInitActor = GetPlayerState<IInitializable>();
	/*if (PostInitActor)
	{
		PostInitActor->CallInitialize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] PlayerState are not exist!!"));
	}*/

	PostInitActor = Cast<IInitializable>(GI->GetMapBoss());
	if (PostInitActor)
	{
		PostInitActor->CallInitialize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] MapBoss are not exist!!"));
	}
}
