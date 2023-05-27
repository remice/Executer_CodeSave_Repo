// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Initializable.h"
#include "AITaskManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXECUTER_API UAITaskManager : public UActorComponent, public IInitializable
{
	GENERATED_BODY()

public:	
	UAITaskManager();

protected:
	virtual void BeginPlay() override;

	virtual void Initialize() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool FindPlayerPawn(float CheckRadius, AActor* OutPawn);
		
};
