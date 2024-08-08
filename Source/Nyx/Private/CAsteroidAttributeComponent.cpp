// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidAttributeComponent.h"

UCAsteroidAttributeComponent::UCAsteroidAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseHealth = 100.0f;
	BaseDamagePower = 10.0f;
	BasePhysicalPower = 50.0f;

	// These default values will be overwritten on init based on BP values
	Size = EAsteroidSize::Base;
	SizeMultiplier = 1.0f;
	AttributeModifier = 1.0f;
	Health = -1.0f;
	DamagePower = -1.0f;

	TimeToActivate = 5;
	NumberOfAsteroidsToSpawn = 3;
}
void UCAsteroidAttributeComponent::InitializeAttributes()
{
	SizeMultiplier = Size.GetIntValue();
	HealthMax = BaseHealth * SizeMultiplier * AttributeModifier;
	Health = HealthMax;
	DamagePower = BaseDamagePower * SizeMultiplier * AttributeModifier;
}
void UCAsteroidAttributeComponent::ModifyAttributes(float NewModifier)
{
	AttributeModifier = NewModifier;
	InitializeAttributes();
}

float UCAsteroidAttributeComponent::GetDamagePower() const
{
	return DamagePower;
}

float UCAsteroidAttributeComponent::GetScaledPhysicalPower()
{
	return Size.GetIntValue() * BasePhysicalPower;
}

EAsteroidSize UCAsteroidAttributeComponent::GetSize() const
{
	return Size;
}

uint8 UCAsteroidAttributeComponent::GetNumberOfAsteroidsToSpawn() const
{
	return NumberOfAsteroidsToSpawn;
}

void UCAsteroidAttributeComponent::SetNumberOfAsteroidsToSpawn(uint8 Num)
{
	NumberOfAsteroidsToSpawn = Num;
}

float UCAsteroidAttributeComponent::GetAttributeModifier() const
{
	return AttributeModifier;
}
