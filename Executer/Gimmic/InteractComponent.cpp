// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/InteractComponent.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UInteractComponent::OnInteract()
{
	K2_OnInteract();
}
