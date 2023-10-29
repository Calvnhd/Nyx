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

void UCAttributeComponent::ApplyHealthChange(float Delta) 
{
	if (Health == 0)
	{
		return;
	}

	float NewHealth = Health + Delta;
	if (NewHealth <= 0)
	{
		Health = 0;
		// fire a delegate?
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
	OnHealthChangedDelegate.Broadcast(Delta, Health);
}

float UCAttributeComponent::GetThrust()
{
	return ThrustPercent;
}