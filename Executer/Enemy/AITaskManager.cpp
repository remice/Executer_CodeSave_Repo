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
