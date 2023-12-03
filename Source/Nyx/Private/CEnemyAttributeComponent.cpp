// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyAttributeComponent.h"

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
