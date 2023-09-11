// Copyright (C) 2023 - Calvin Davidson

#include "PAttributeComponent.h"

// Sets default values for this component's properties
UPAttributeComponent::UPAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	HealthMax = 1000.0f;
	Health = HealthMax;

	Thrust = 0.0f;
	ThrustMax = 1000.0f;
	ThrustQuanta = ThrustMax * 0.01;
}

float UPAttributeComponent::GetThrust()
{
	return Thrust;
}

void UPAttributeComponent::IncreaseThrust()
{
	// todo
}

void UPAttributeComponent::DecreaseThrust()
{
	// todo
}

void UPAttributeComponent::SetThrustToMax()
{
	Thrust = ThrustMax;
}

void UPAttributeComponent::SetThrustToMin()
{
	Thrust = 0.0f;
}
