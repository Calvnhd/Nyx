// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyAttributeComponent.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCEnemyAttributeComponent::UCEnemyAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Health = HealthMax;
}

bool UCEnemyAttributeComponent::IsAlive()
{
	return (Health > 0);
}
// todo -- this should take an InstigatorActor
void UCEnemyAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (Health == 0)
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
										 FString::Printf(TEXT("Health changed by %f, health now %f"), Delta, Health));
	}
	OnHealthChangedDelegate.Broadcast(InstigatorActor, this, Delta, Health);
}

float UCEnemyAttributeComponent::GetHealth()
{
	return Health;
}

float UCEnemyAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

float UCEnemyAttributeComponent::GetHealthPercent()
{
	return (Health / HealthMax);
}

float UCEnemyAttributeComponent::GetCollisionDamageAmount()
{
	return CollisionDamageAmount;
}

bool UCEnemyAttributeComponent::Kill(AActor* InstigatorActor)
{
	ApplyHealthChange(InstigatorActor, -HealthMax);
	return true;
}

UCEnemyAttributeComponent* UCEnemyAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCEnemyAttributeComponent>(
			FromActor->GetComponentByClass(UCEnemyAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UCEnemyAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (UCEnemyAttributeComponent* AttributeComp = GetAttributes(Actor))
	{
		return AttributeComp->IsAlive();
	}
	// This implies that having no attribute component is the equivalent of being dead
	return false;
}
