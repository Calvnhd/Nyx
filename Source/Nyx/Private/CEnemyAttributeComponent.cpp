// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCEnemyAttributeComponent::UCEnemyAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;
}

bool UCEnemyAttributeComponent::IsAlive()
{
	return (Health > 0);
}

void UCEnemyAttributeComponent::ApplyHealthChange(float Delta) 
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Health changed by %f, health now %f"), Delta, Health));
	}
	OnEnemyHealthChangedDelegate.Broadcast(Delta, Health);
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
