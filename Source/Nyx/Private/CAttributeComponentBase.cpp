// Copyright (C) 2023 - Calvin Davidson

#include "CAttributeComponentBase.h"

#include "Math/UnrealMathUtility.h"

UCAttributeComponentBase::UCAttributeComponentBase()
{
	HealthMax = 1000.0f;
	Health = HealthMax;
}
UCAttributeComponentBase* UCAttributeComponentBase::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCAttributeComponentBase>(FromActor->GetComponentByClass(UCAttributeComponentBase::StaticClass()));
	}
	return nullptr;
}
bool UCAttributeComponentBase::IsActorAlive(AActor* Actor)
{
	if (UCAttributeComponentBase* AttributeComp = GetAttributes(Actor))
	{
		return AttributeComp->IsAlive();
	}
	// This implies that having no attribute component is the equivalent of being dead
	return false;
}
bool UCAttributeComponentBase::IsAlive()
{
	return (Health > 0);
}
void UCAttributeComponentBase::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() || Health <= 0)
	{
		return;
	}

	float NewHealth = Health + Delta;
	if (NewHealth <= 0)
	{
		Health = 0;
	}
	else if (NewHealth >= HealthMax)
	{
		Health = HealthMax;
	}
	else
	{
		Health = NewHealth;
	}
	OnHealthChangedDelegate.Broadcast(InstigatorActor, this, Delta, Health);
}
float UCAttributeComponentBase::GetHealth()
{
	return Health;
}
float UCAttributeComponentBase::GetHealthMax()
{
	return HealthMax;
}
float UCAttributeComponentBase::GetHealthPercent()
{
	return (Health / HealthMax);
}
