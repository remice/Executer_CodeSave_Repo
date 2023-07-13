// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInteractManager.generated.h"

DECLARE_DELEGATE_OneParam(FOnHitSignature, bool /*bIsIn*/)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UCharacterInteractManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterInteractManager();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetEnable(bool IsEnable);
	bool OnInteract();
	void InitManager(USceneComponent* InCamera, float InCheckLength);

private:
	void CheckCollide();
	void SetCollideInfo(bool IsCollide);

public:
	FOnHitSignature OnHit;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Camera;

	class IInteractableObject* SelectedObject;

	float CheckLength;
	uint8 bSaveCollideInfo : 1;
};