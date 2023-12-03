// Copyright (C) 2023 - Calvin Davidson

#include "CAttributeComponent.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCAttributeComponent::UCAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;

	ThrustPercent = 0;
	ThrustPercentMax = 100;
	SpeedMax = 5000;
}

UCAttributeComponent* UCAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<UCAttributeComponent>(FromActor->GetComponentByClass(UCAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UCAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (UCAttributeComponent* AttributeComp = GetAttributes(Actor))
	{
		return AttributeComp->IsAlive();
	}
	// This implies that having no attribute component is the equivalent of being dead
	return false;
}

float UCAttributeComponent::GetSpeed()
{
	return (ThrustPercent / ThrustPercentMax) * SpeedMax;
}

void UCAttributeComponent::IncrementThrust()
{
	ThrustPercent++;
	ThrustPercent = FMath::Clamp(ThrustPercent, 0, ThrustPercentMax);
}
void UCAttributeComponent::DecrementThrust()
{
	ThrustPercent--;
	ThrustPercent = FMath::Clamp(ThrustPercent, 0, ThrustPercentMax);
}

bool UCAttributeComponent::IsAlive()
{
	return (Health > 0);
}

// todo, should return bool?
void UCAttributeComponent::ApplyHealthChange(float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return;
	}

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
		GEngine->AddOnScreenDebugMessage(
			-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("Player health changed by %f, health now %f"), Delta, Health));
	}
	OnPlayerHealthChangedDelegate.Broadcast(nullptr, this, Delta, Health);
}

float UCAttributeComponent::GetHealth()
{
	return Health;
}

float UCAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

float UCAttributeComponent::GetHealthPercent()
{
	return (Health / HealthMax);
}

float UCAttributeComponent::GetThrust()
{
	return ThrustPercent;
}