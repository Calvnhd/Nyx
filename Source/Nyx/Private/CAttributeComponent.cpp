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

int UCAttributeComponent::GetThrust()
{
	return ThrustPercent;
}