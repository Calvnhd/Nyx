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
	Health = -1.0f;
	Power = -1.0f;
}
void UCAsteroidAttributeComponent::InitializeAttributes()
{
	SizeMultiplier = Size.GetIntValue();
	HealthMax = BaseHealth * SizeMultiplier;
	Health = HealthMax;
	Power = BasePower * SizeMultiplier;
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
	switch (Size)
	{
		case EAsteroidSize::Base:
			return 0;
		case EAsteroidSize::Small:
			return 6;
		case EAsteroidSize::Medium:
			return 5;
		case EAsteroidSize::Large:
			return 4;
		case EAsteroidSize::Largest:
			return 3;
		default:
			return {};
	}
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
