// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InteractableObject.h"

#include "InteractComponent.generated.h"


UCLASS( Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UInteractComponent : public UActorComponent, public IInteractableObject
{
	GENERATED_BODY()

public:	
	UInteractComponent();
		
public:
	virtual void OnInteract() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom", meta = (DisplayName = "On_Interact"))
	void K2_OnInteract();
};
