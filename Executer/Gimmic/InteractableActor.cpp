// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmic/InteractableActor.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AInteractableActor::OnInteract()
{
	K2_OnInteract();
}