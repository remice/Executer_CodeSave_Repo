// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#define PATH_IMC TEXT("/Game/Input/IMC_PlayerDefault.IMC_PlayerDefault")
#define PATH_IA_LOOK TEXT("/Game/Input/Action/IA_Look.IA_Look")
#define PATH_IA_MOVE TEXT("/Game/Input/Action/IA_Move.IA_Move")
#define PATH_IA_JUMP TEXT("/Game/Input/Action/IA_Jump.IA_Jump")
#define PATH_IA_DASH TEXT("/Game/Input/Action/IA_Dash.IA_Dash")
#define PATH_IA_CAMERAAUTO TEXT("/Game/Input/Action/IA_CameraAuto.IA_CameraAuto")
#define PATH_IA_CAMERAFIXED TEXT("/Game/Input/Action/IA_CameraFixed.IA_CameraFixed")

AMainPlayerController::AMainPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(PATH_IMC);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_LOOK(PATH_IA_LOOK);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(PATH_IA_MOVE);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_JUMP(PATH_IA_JUMP);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DASH(PATH_IA_DASH);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CAMERAAUTO(PATH_IA_CAMERAAUTO);
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_CAMERAFIXED(PATH_IA_CAMERAFIXED);

	check(IMC.Succeeded());
	check(IA_LOOK.Succeeded());
	check(IA_MOVE.Succeeded());
	check(IA_JUMP.Succeeded());
	check(IA_DASH.Succeeded());
	check(IA_CAMERAAUTO.Succeeded());
	check(IA_CAMERAFIXED.Succeeded());

	PlayerMappingContext = IMC.Object;
	LookAction = IA_LOOK.Object;
	MoveAction = IA_MOVE.Object;
	JumpAction = IA_JUMP.Object;
	DashAction = IA_DASH.Object;
	CameraAutoAction = IA_CAMERAAUTO.Object;
	CameraFixedAction = IA_CAMERAFIXED.Object;
}