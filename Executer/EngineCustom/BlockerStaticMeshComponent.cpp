// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineCustom/BlockerStaticMeshComponent.h"

UBlockerStaticMeshComponent::UBlockerStaticMeshComponent()
{
	SetCollisionProfileName("Blocker");
	
	OnComponentHit.AddDynamic(this, &UBlockerStaticMeshComponent::OnCompHit);

	MaxHp = 100;
	CurHp = 100;
}

void UBlockerStaticMeshComponent::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

bool UBlockerStaticMeshComponent::EvaluationDamage(int32 DamageLevel, float DamageAmount)
{
	if (DamageLevel > BlockerLevel)
	{
		ChangeHp(0);
		return true;
	}
	else if (DamageLevel == BlockerLevel)
	{
		ChangeHp(CurHp - FMath::Clamp(DamageAmount, 0, DamageAmount));
		return false;
	}
	return false;
}

void UBlockerStaticMeshComponent::ChangeHp(const float InCurHp)
{
	CurHp = FMath::Clamp(InCurHp, 0, MaxHp);

	if (CurHp == 0)
	{
		OnDead.ExecuteIfBound();
	}
}
