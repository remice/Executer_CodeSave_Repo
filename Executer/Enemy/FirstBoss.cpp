// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FirstBoss.h"
#include "AI/FirstBossAIController.h"

#define PATH_ANIMINSTANCE_C TEXT("/Game/Character/Enemy/ABP_Gideon.ABP_Gideon_c")
#define PATH_SKELETALMESH TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Skins/Inquisitor/Meshes/Gideon_Inquisitor.Gideon_Inquisitor'")

AFirstBoss::AFirstBoss()
{
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(PATH_ANIMINSTANCE_C);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(PATH_SKELETALMESH);

	ensure(AnimInstanceRef.Succeeded());
	ensure(SkeletalMeshRef.Succeeded());

	Mesh->SetAnimInstanceClass(AnimInstanceRef.Class);
	Mesh->SetSkeletalMesh(SkeletalMeshRef.Object);

	AIControllerClass = AFirstBossAIController::StaticClass();
}
