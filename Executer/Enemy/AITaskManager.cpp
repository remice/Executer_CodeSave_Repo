// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AITaskManager.h"
#include "Kismet/KismetSystemLibrary.h"

UAITaskManager::UAITaskManager()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAITaskManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAITaskManager::Initialize()
{
}

void UAITaskManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UAITaskManager::FindPlayerPawn(float CheckRadius, AActor* OutPawn)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActorArray;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	FHitResult HitResult;
	bool OnCollide = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation(),
		CheckRadius, ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, HitResult, true);

	if (OnCollide == false)
	{
		OutPawn = nullptr;
		return false;
	}

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

	for (int ZMultiplier = -1; ZMultiplier <= 1; ZMultiplier++)
	{
		FHitResult FindResult;
		bool OnFindPawn = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(), 
			GetOwner()->GetActorLocation() + FVector(0, 0, 44.f), 
			HitResult.GetActor()->GetActorLocation() + (FVector(0, 0, 44.f) * ZMultiplier),
			ObjectTypesArray, false, IgnoreActorArray, EDrawDebugTrace::None, FindResult, true);

		if (OnFindPawn)
		{
			OutPawn = FindResult.GetActor();
			return true;
		}
	}

	OutPawn = nullptr;
	return false;
}

