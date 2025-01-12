// Copyright (C) 2024 - Calvin Davidson

#include "AI/CEnemyAttributeComponent.h"

#include "Math/UnrealMathUtility.h"

UCEnemyAttributeComponent::UCEnemyAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
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
