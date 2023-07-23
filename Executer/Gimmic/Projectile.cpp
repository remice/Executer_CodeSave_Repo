// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "EngineCustom/BlockerStaticMeshComponent.h"

#define PATH_SPHERE TEXT("/Engine/BasicShapes/Sphere.Sphere")

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set default Subobjects
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	// Set mesh to sphere(default)
	ConstructorHelpers::FObjectFinder<UStaticMesh> M_SPHERE(PATH_SPHERE);
	check(M_SPHERE.Succeeded());

	// Init collision setting
	Body->SetStaticMesh(M_SPHERE.Object);
	Body->SetRelativeScale3D(FVector3d(.5f, .5f, .5f));
	Body->SetSimulatePhysics(true);
	Body->SetEnableGravity(false);
	Body->SetCollisionProfileName(TEXT("Projectile"));
	SetRootComponent(Body);

	// Set Overlap Event
	Body->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapPlayer);

	// Init movemnet setting
	ProjectileMovement->InitialSpeed = 800.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 5.f;

	rAttackType = EAttackData::Projectile;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::DestroySelf()
{
	SpawnHitEffect();
	Super::DestroySelf();
}

void AProjectile::OnOverlapPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UBlockerStaticMeshComponent* BlockerMeshComponent = Cast<UBlockerStaticMeshComponent>(OtherComp);
	if (BlockerMeshComponent)
	{
		BlockerMeshComponent->EvaluationDamage(GetAttackLevel(), GetDamage());
	}

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(GetDamage(), DamageEvent, nullptr, this);
	DestroySelf();
}

int32 AProjectile::GetId()
{
	return rId;
}

void AProjectile::SpawnHitEffect()
{
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, GetActorLocation(), FRotator(0.f));
	}
}
