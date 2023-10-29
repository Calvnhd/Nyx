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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ApplyHealthChange: %s"), Delta));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Health: %s"), Health));
	}
}

float UCAttributeComponent::GetThrust()
{
	return ThrustPercent;
}