// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyAttributeComponent.h"
//#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCEnemyAttributeComponent::UCEnemyAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;
}

float UCEnemyAttributeComponent::GetHealth()
{
	return Health;
}
void UCEnemyAttributeComponent::ApplyDamage(float DamageAmount)
{
	if (Health == 0)
	{
		return;
	}

	float NewHealth = Health - DamageAmount;
	
	if (NewHealth <= 0)
	{
		Health = 0;
	}
	else
	{
		Health = NewHealth;
	}
}