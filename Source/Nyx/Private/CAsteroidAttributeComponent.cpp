// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidAttributeComponent.h"

UCAsteroidAttributeComponent::UCAsteroidAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	BaseHealth = 100.0f;
	BasePower = 10.0f;

	// These default values will be overwritten on init based on BP values
	Size = EAsteroidSize::Base;
	SizeMultiplier = 1.0f;
	AttributeModifier = 1.0f;
	Health = -1.0f;
	Power = -1.0f;

	TimeToActivate = 5;
	NumberOfAsteroidsToSpawn = 3;
}
void UCAsteroidAttributeComponent::InitializeAttributes()
{
	SizeMultiplier = Size.GetIntValue();
	HealthMax = BaseHealth * SizeMultiplier * AttributeModifier;
	Health = HealthMax;
	Power = BasePower * SizeMultiplier * AttributeModifier;
}
void UCAsteroidAttributeComponent::ModifyAttributes(float NewModifier)
{
	AttributeModifier = NewModifier;
	InitializeAttributes();
}

float UCAsteroidAttributeComponent::GetPower() const
{
	return Power;
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

//UCAsteroidAttributeComponent* UCAsteroidAttributeComponent::GetAttributes(AActor* FromActor)
//{
//	if (FromActor)
//	{
//		return Cast<UCAsteroidAttributeComponent>(
//			FromActor->GetComponentByClass(UCAsteroidAttributeComponent::StaticClass()));
//	}
//	return nullptr;
//}
//bool UCAsteroidAttributeComponent::IsAsteroidAlive(AActor* Actor)
//{
//	if (UCAsteroidAttributeComponent* AttributeComp = GetAttributes(Actor))
//	{
//		return AttributeComp->IsAlive();
//	}
//	// This implies that having no attribute component is the equivalent of being dead
//	return false;
//}
//bool UCAsteroidAttributeComponent::IsAlive() const
//{
//	return Health > 0;
//}
//float UCAsteroidAttributeComponent::GetHealth() const
//{
//	return Health;
//}
//
//float UCAsteroidAttributeComponent::GetHealthPercent() const
//{
//	return Health / HealthMax;
//}
//void UCAsteroidAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
//{
//	if (!GetOwner()->CanBeDamaged() || Health <= 0)
//	{
//		return;
//	}
//
//	float NewHealth = Health + Delta;
//	if (NewHealth <= 0)
//	{
//		Health = 0;
//	}
//	else if (NewHealth >= HealthMax)
//	{
//		Health = HealthMax;
//	}
//	else
//	{
//		Health = NewHealth;
//	}
//	OnHealthChanged.Broadcast(InstigatorActor, this, Delta, Health);
//}
