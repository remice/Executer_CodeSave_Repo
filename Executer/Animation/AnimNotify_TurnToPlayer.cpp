// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_TurnToPlayer.h"
#include "Interface/ExcuterAIInterface.h"

UAnimNotify_TurnToPlayer::UAnimNotify_TurnToPlayer()
{
	InterpSpeed = 60.f;
}

void UAnimNotify_TurnToPlayer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UWorld* World = MeshComp->GetWorld();
	APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	AActor* Player = Controller ? Controller->GetPawn() : nullptr;
	if (IsValid(Player) == false) return;

	IExcuterAIInterface* AIPawn = Cast<IExcuterAIInterface>(MeshComp->GetOwner());
	if (AIPawn)
	{
		AIPawn->TurnToLoc(Player->GetActorLocation(), InterpSpeed);
	}
}
