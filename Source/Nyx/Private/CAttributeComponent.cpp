// Copyright (C) 2023 - Calvin Davidson

#include "CAttributeComponent.h"

// Sets default values for this component's properties
UCAttributeComponent::UCAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;

	Thrust = 0.0f;
	ThrustMax = 1000.0f;
	ThrustQuanta = ThrustMax * 0.01;
}

void UCAttributeComponent::IncreaseThrust()
{
	Thrust = ThrustMax;
}

void UCAttributeComponent::DecreaseThrust()
{
	Thrust = 0.0f;
}

float UCAttributeComponent::GetThrust()
{
	return Thrust;
}