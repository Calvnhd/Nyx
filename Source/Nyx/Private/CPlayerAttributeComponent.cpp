// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerAttributeComponent.h"

#include "CSkillPointsPickup.h"

UCPlayerAttributeComponent::UCPlayerAttributeComponent()
{
	HealthMax = 1000.0f;
	Health = HealthMax;
	SkillPoints = 0.0f;
	MaxBoost = 300.0f;
	CurrentBoost = MaxBoost;
	BoostToDash = 100.0f;
	BoostToJump = 100.0f;
	BoostRecoveryCooldownTime = 1.0f;
	BoostRecoveryIncrement = 10.0f;
	BoostRecoveryRate = 0.2f;
}

void UCPlayerAttributeComponent::AddSkillPoints(float PointsToAdd)
{
	SkillPoints += PointsToAdd;
	OnSkillPointsChanged.Broadcast(this, PointsToAdd, SkillPoints);
}

float UCPlayerAttributeComponent::GetSkillPoints() const
{
	return SkillPoints;
}

float UCPlayerAttributeComponent::ConsumeDashBoost()
{
	float BoostModifier = 0.0f;
	if (CurrentBoost >= BoostToDash)
	{
		BoostModifier = 1.0f;
		CurrentBoost -= BoostToDash;
	}
	else if (CurrentBoost <= BoostToDash && CurrentBoost > 0)
	{
		BoostModifier = CurrentBoost / BoostToDash;
		CurrentBoost = 0.0f;
	}
	if (BoostModifier > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(BoostRecoveryCooldownTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost,
											   BoostRecoveryCooldownTime);
	}
	return BoostModifier;
}
float UCPlayerAttributeComponent::ConsumeJumpBoost()
{
	float BoostModifier = 0.0f;
	if (CurrentBoost >= BoostToJump)
	{
		BoostModifier = 1.0f;
		CurrentBoost -= BoostToJump;
	}
	else if (CurrentBoost <= BoostToJump && CurrentBoost > 0)
	{
		BoostModifier = CurrentBoost / BoostToJump;
		CurrentBoost = 0.0f;
	}
	if (BoostModifier > 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BoostRecoveryTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(BoostRecoveryCooldownTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost,
											   BoostRecoveryCooldownTime);
	}
	return BoostModifier;
}

void UCPlayerAttributeComponent::RecoverBoost()
{
	float NewBoost = CurrentBoost + BoostRecoveryIncrement;
	if (NewBoost > MaxBoost)
	{
		CurrentBoost = MaxBoost;
		return;
	}
	CurrentBoost = NewBoost;
	GetWorld()->GetTimerManager().SetTimer(BoostRecoveryTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost,
										   BoostRecoveryRate);
}
