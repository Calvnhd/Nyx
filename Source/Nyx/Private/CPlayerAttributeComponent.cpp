// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerAttributeComponent.h"

#include "Math/UnrealMathUtility.h"

UCPlayerAttributeComponent::UCPlayerAttributeComponent()
{
	HealthMax = 1000.0f;
	Health = HealthMax;
}