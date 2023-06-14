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
	HudWidgetClass = HUDWIDGET_C.Class;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	InitHUDWidget();
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

	IInitializable* PreInitActor = GetPlayerState<IInitializable>();
	if (PreInitActor)
	{
		PreInitActor->CallInitialize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] PlayerState are not exist!!"));
	}

	PreInitActor = Cast<IInitializable>(GI->GetMapBoss());
	if (PreInitActor)
	{
		PreInitActor->CallInitialize();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[MainPlayerController] MapBoss are not exist!!"));
	}
}
