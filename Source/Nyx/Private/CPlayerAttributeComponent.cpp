// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerAttributeComponent.h"

UCPlayerAttributeComponent::UCPlayerAttributeComponent()
{
	HealthMax = 1000.0f;
	Health = HealthMax;
	SkillPoints = 0.0f;
}

void UCPlayerAttributeComponent::AddSkillPoints(float PointsToAdd)
{
	SkillPoints += PointsToAdd;
}

float UCPlayerAttributeComponent::GetSkillPoints() const
{
	return SkillPoints;
}