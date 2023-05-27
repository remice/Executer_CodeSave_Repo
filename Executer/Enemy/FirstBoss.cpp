// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/FirstBoss.h"
#include "AI/FirstBossAIController.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "AITaskManager.h"

#define PATH_ANIMINSTANCE_C TEXT("/Game/Character/Enemy/ABP_Gideon.ABP_Gideon_c")
#define PATH_SKELETALMESH TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Skins/Inquisitor/Meshes/Gideon_Inquisitor.Gideon_Inquisitor'")
#define PATH_PHYSMATERIAL TEXT("/Script/PhysicsCore.PhysicalMaterial'/Game/Character/Enemy/PM_Gideon.PM_Gideon'")

AFirstBoss::AFirstBoss()
{
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(PATH_ANIMINSTANCE_C);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshRef(PATH_SKELETALMESH);
	ConstructorHelpers::FObjectFinder<UPhysicalMaterial> PhysMaterialRef(PATH_PHYSMATERIAL);

	ensure(AnimInstanceRef.Succeeded());
	ensure(SkeletalMeshRef.Succeeded());
	ensure(PhysMaterialRef.Succeeded());

	Mesh->SetAnimInstanceClass(AnimInstanceRef.Class);
	Mesh->SetSkeletalMesh(SkeletalMeshRef.Object);

	GetCollider()->SetPhysMaterialOverride(PhysMaterialRef.Object);

	AIControllerClass = AFirstBossAIController::StaticClass();

	AITaskManager = CreateDefaultSubobject<UAITaskManager>(TEXT("AITaskManager"));
}
