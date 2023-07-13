// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableObject.h"

#include "InteractableActor.generated.h"

UCLASS(BlueprintType)
class EXECUTER_API AInteractableActor : public AActor, public IInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

public:
	virtual void OnInteract() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "CustomActor", meta = (DisplayName = "On_Interact"))
	void K2_OnInteract();
};
