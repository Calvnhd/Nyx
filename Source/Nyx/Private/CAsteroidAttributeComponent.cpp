// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidAttributeComponent.h"

UCAsteroidAttributeComponent::UCAsteroidAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Size = EAsteroidSize::Base;
	SizeMultiplier = 1.0f;

	State = EAsteroidState::Dormant;

	BaseHealth = 100.0f;
	BaseDamagePower = 10.0f;
	BasePhysicalPower = 50.0f;

	ChanceToSpawnItem = 1.0f;
	TimeToActivate = 5;
	NumberOfAsteroidsToSpawn = 3;

	AttributeModifier = 1.0f;
	Health = -1.0f;
	DamagePower = -1.0f;
	InitializeAttributes();
}
void UCAsteroidAttributeComponent::InitializeAttributes()
{
	SizeMultiplier = Size.GetIntValue();
	HealthMax = BaseHealth * SizeMultiplier * AttributeModifier;
	Health = HealthMax;
	DamagePower = BaseDamagePower * SizeMultiplier * AttributeModifier;
	State = EAsteroidState::Dormant;
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

EAsteroidState UCAsteroidAttributeComponent::GetState() const
{
	return State;
}

void UCAsteroidAttributeComponent::SetState(EAsteroidState NewState)
{
	State = NewState;
}

uint8 UCAsteroidAttributeComponent::GetNumberOfAsteroidsToSpawn() const
{
	return NumberOfAsteroidsToSpawn;
}

void UCAsteroidAttributeComponent::SetNumberOfAsteroidsToSpawn(uint8 Num)
{
	NumberOfAsteroidsToSpawn = Num;
}

float UCAsteroidAttributeComponent::GetChanceToSpawnItem() const
{
	return ChanceToSpawnItem;
}

float UCAsteroidAttributeComponent::GetAttributeModifier() const
{
	return AttributeModifier;
}

bool UCAsteroidAttributeComponent::TrySpawnItem() const
{
	if (ChanceToSpawnItem == 1)
	{
		return true;
	}
	if (ChanceToSpawnItem == 0)
	{
		return false;
	}
	return (FMath::RandRange(0.0f, 1.0f) <= ChanceToSpawnItem);
}
