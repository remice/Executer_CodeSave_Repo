// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/PatternBase.h"
#include "Components/ArrowComponent.h"

// Sets default values
APatternBase::APatternBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<UArrowComponent>(TEXT("Body"));
	Body->ArrowSize = 1.5f;
	Body->ArrowColor = FColor::Cyan;
	SetRootComponent(Body);

	MaxPatternTime = 10.f;
	DestroyTime = 100.f;
	CurPatternTime = 0.f;
	PatternId = 0;
}

// Called when the game starts or when spawned
void APatternBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(DestroyTime);

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
void APatternBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnFire == false)
	{
		return;
	}

	CurPatternTime += DeltaTime;
	if (CurPatternTime < MaxPatternTime)
	{
		ExFire();
	}
	else
	{
		bOnFire = false;
		// Callback shot end event
		if (OnEnd.IsBound())
		{
			OnEnd.Execute();
		}
	}
}

void APatternBase::SetupPattern(AActor* InPlayerCharacter)
{
	PlayerCharacter = InPlayerCharacter;
}

void APatternBase::Fire()
{
	bOnFire = true;
}

void APatternBase::ExFire()
{
	return;
}

// Spawn bullets at all points
// [params] BulletIndex >> Select bullet in bullet array
void APatternBase::SpawnBullets()
{
	return;
}