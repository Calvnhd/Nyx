// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerAttributeComponent.h"

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
	BoostRecoveryIncrementBase = 10.0f;
	CurrentBoostRecoveryIncrement = BoostRecoveryIncrementBase;
	BoostRecoveryIncrementModifier = 1.05;
	BoostRecoveryRate = 0.2f;
	bIsInvulnerable = false;
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
	if (CurrentBoost <= 0)
	{
		return 0.0f;
	}
	float BoostModifier = 0.0f;
	if (CurrentBoost >= BoostToDash)
	{
		BoostModifier = 1.0f;
	}
	else
	{
		BoostModifier = CurrentBoost / BoostToDash;
	}
	CurrentBoostRecoveryIncrement = BoostRecoveryIncrementBase;
	UpdateBoost(-BoostToDash);
	GetWorld()->GetTimerManager().ClearTimer(BoostRecoveryTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BoostRecoveryCooldownTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost,
										   BoostRecoveryCooldownTime);

	return BoostModifier;
}
float UCPlayerAttributeComponent::ConsumeJumpBoost()
{
	if (CurrentBoost <= 0)
	{
		return 0.0f;
	}
	float BoostModifier = 0.0f;
	if (CurrentBoost >= BoostToJump)
	{
		BoostModifier = 1.0f;
	}
	else
	{
		BoostModifier = CurrentBoost / BoostToJump;
	}
	CurrentBoostRecoveryIncrement = BoostRecoveryIncrementBase;
	UpdateBoost(-BoostToJump);
	GetWorld()->GetTimerManager().ClearTimer(BoostRecoveryTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BoostRecoveryCooldownTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost,
										   BoostRecoveryCooldownTime);

	return BoostModifier;
}

float UCPlayerAttributeComponent::GetBoostPercent() const
{
	return CurrentBoost / MaxBoost;
}

void UCPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentBoost = MaxBoost;
	CurrentBoostRecoveryIncrement = BoostRecoveryIncrementBase;
}

void UCPlayerAttributeComponent::RecoverBoost()
{
	if (CurrentBoost < MaxBoost)
	{
		UpdateBoost(CurrentBoostRecoveryIncrement);
		CurrentBoostRecoveryIncrement = CurrentBoostRecoveryIncrement * BoostRecoveryIncrementModifier;
		GetWorld()->GetTimerManager().SetTimer(BoostRecoveryTimerHandle, this, &UCPlayerAttributeComponent::RecoverBoost, BoostRecoveryRate);
	}
	else
	{
		CurrentBoostRecoveryIncrement = BoostRecoveryIncrementBase;
	}
}

void UCPlayerAttributeComponent::UpdateBoost(float Delta)
{
	float NewBoost = CurrentBoost + Delta;
	if (NewBoost < 0.0f)
	{
		CurrentBoost = 0.0f;
	}
	else if (NewBoost > MaxBoost)
	{
		CurrentBoost = MaxBoost;
	}
	else
	{
		CurrentBoost = NewBoost;
	}
	OnBoostChanged.Broadcast(this, CurrentBoost);
}
