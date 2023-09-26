// Copyright (C) 2023 - Calvin Davidson

#include "CAttributeComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UCAttributeComponent::UCAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;

	ThrustPercent = 0.0f;
	ThrustMax = 1000.0f;
}

float UCAttributeComponent::GetThrust()
{
	return (ThrustPercent / 100) * ThrustMax;
}

	void UCAttributeComponent::IncrementThrust()
	{
		ThrustPercent++;
		ThrustPercent = FMath::Clamp(ThrustPercent, 0.0f, 100.0f);
	}
	void UCAttributeComponent::DecrementThrust()
	{
		ThrustPercent--;
		ThrustPercent = FMath::Clamp(ThrustPercent, 0.0f, 100.0f);
	}