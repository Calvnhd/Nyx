// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidAttributeComponent.h"

UCAsteroidAttributeComponent::UCAsteroidAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseHealth = 100.0f;
	BasePower = 100.0f;

	// These default values will be overwritten on init based on BP values 
	Size = EAsteroidSize::Base;
	SizeMultiplier = 1.0f;
	Health = -1.0f;
	Power = -1.0f;
}

void UCAsteroidAttributeComponent::InitializeAttributes()
{
	SizeMultiplier = Size.GetIntValue();
	Health = BaseHealth * SizeMultiplier;
	Power = BasePower * SizeMultiplier;
}
void UCAsteroidAttributeComponent::TakeDamage(float Damage)
{
	Health -= Damage;
}
bool UCAsteroidAttributeComponent::IsAlive() const
{
	return Health > 0;
}
