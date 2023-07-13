// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterInteractManager.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Interface/InteractableObject.h"

// Sets default values for this component's properties
UCharacterInteractManager::UCharacterInteractManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	CheckLength = 200.f;
	bSaveCollideInfo = false;
}


// Called every frame
void UCharacterInteractManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCollide();
}

void UCharacterInteractManager::SetEnable(bool IsEnable)
{
	PrimaryComponentTick.SetTickFunctionEnable(IsEnable);
}

bool UCharacterInteractManager::OnInteract()
{
	if (SelectedObject != nullptr)
	{
		SelectedObject->OnInteract();
		return true;
	}

	return false;
}

void UCharacterInteractManager::InitManager(USceneComponent* InCamera, float InCheckLength)
{
	Camera = InCamera;
	CheckLength = InCheckLength;
}

void UCharacterInteractManager::CheckCollide()
{
	FVector StartPoint = Camera->GetComponentLocation() + Camera->GetForwardVector() * 50.f;
	FVector EndPoint = StartPoint + Camera->GetForwardVector() * CheckLength;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoreActorArray;

	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypesArray.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	IgnoreActorArray.Emplace(GetOwner());

	FHitResult HitResult;
	bool IsCollide = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartPoint, EndPoint, ObjectTypesArray, false,
		IgnoreActorArray, EDrawDebugTrace::None, HitResult, false);

	if (IsCollide == false)
	{
		SetCollideInfo(false);
		return;
	}

	IInteractableObject* InteractableObject = Cast<IInteractableObject>(HitResult.GetActor());
	if (InteractableObject)
	{
		SelectedObject = InteractableObject;
		SetCollideInfo(true);
		return;
	}

	SetCollideInfo(false);
}

void UCharacterInteractManager::SetCollideInfo(bool IsCollide)
{
	if (bSaveCollideInfo == IsCollide) return;
	bSaveCollideInfo = IsCollide;
	OnHit.ExecuteIfBound(bSaveCollideInfo);

	if (bSaveCollideInfo == false)
	{
		SelectedObject = nullptr;
	}
}