// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePattern.h"
#include "Interface/IRotatableObject.h"
#include "Components/ArrowComponent.h"
#include "Gimmic/Projectile.h"

// Sets default values
ABasePattern::ABasePattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UArrowComponent>(TEXT("Body"));
	Body->ArrowSize = 1.5f;
	Body->ArrowColor = FColor::Cyan;
	SetRootComponent(Body);

	CurPatternTime = 0.f;
	PatternIndex = 0;
	PatternId = 0;
}

// Called when the game starts or when spawned
void ABasePattern::BeginPlay()
{
	Super::BeginPlay();

	if (DelayArray.IsEmpty() || BulletArray.IsEmpty())
	{
		Destroy();
	}

	TArray<TObjectPtr<USceneComponent>> ChildrenArray = Body->GetAttachChildren();

	for (const TObjectPtr<USceneComponent> Child : ChildrenArray)
	{
		UArrowComponent* Arrow = Cast<UArrowComponent>(Child);
		if (Arrow)
		{
			FirePointArray.Add(Arrow);
		}
	}
}

// Called every frame
void ABasePattern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnFire == false)
	{
		return;
	}

	CurPatternTime += DeltaTime;
	if (CurPatternTime > DelayArray[PatternIndex])
	{
		ExFire();
	}
}

void ABasePattern::SetupPattern(AActor* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void ABasePattern::Fire()
{
	PatternIndex = 0;

	bOnFire = true;
}

void ABasePattern::ExFire()
{
	// Select bullet
	int32 BulletIndex = PatternIndex;
	while (BulletIndex >= BulletArray.Num())
	{
		BulletIndex -= BulletArray.Num();
	}

	SpawnBullets(BulletIndex);

	CurPatternTime = 0.f;

	PatternIndex++;

	// Is end pattern?
	bool IsOverLength = (PatternIndex >= DelayArray.Num());
	if (IsOverLength)
	{
		bOnFire = false;
		// Callback shot end event
		if (OnEnd.IsBound())
		{
			OnEnd.Execute();
		}
		return;
	}
}

// Spawn bullets at all points
// [params] BulletIndex >> Select bullet in bullet array
void ABasePattern::SpawnBullets(int32 BulletIndex)
{
	for (const TObjectPtr<UArrowComponent> FirePoint : FirePointArray)
	{
		if (IsValid(BulletArray[BulletIndex]) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("BasePattern/BulletArray[BulletIndex] was invalid, check property"));
			continue;
		}

		IIRotatableObject* RotatableObject = Cast<IIRotatableObject>(FirePoint);

		if (RotatableObject)
		{
			RotatableObject->RelativeRotate(CurPatternTime);
		}

		FVector3d SpawnLoc = FirePoint->GetComponentLocation();
		FRotator3d SpawnRot = FirePoint->GetComponentRotation();

		FActorSpawnParameters SpawnParameters;
		AActor* SpawnObject = GetWorld()->SpawnActor(BulletArray[BulletIndex], &SpawnLoc, &SpawnRot, SpawnParameters);
		AProjectile* Projectile = Cast<AProjectile>(SpawnObject);
		if (Projectile == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("BasePattern/Cast to Projectil was failed."));
		}
		Projectile->SetId(PatternId);
		PatternId++;
	}
}
