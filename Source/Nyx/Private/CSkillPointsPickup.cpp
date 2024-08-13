// Copyright (C) 2024 - Calvin Davidson

#include "CSkillPointsPickup.h"

#include "CPlayerAttributeComponent.h"

ACSkillPointsPickup::ACSkillPointsPickup()
{
	PointsValue = 10.0f;
	bCanSuction = true;
}

void ACSkillPointsPickup::SetCanSuction(bool bNewCanSuction)
{
	bCanSuction = bNewCanSuction;
}

void ACSkillPointsPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACSkillPointsPickup::ConsumePickup_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		if (UCPlayerAttributeComponent* Attributes =
				Cast<UCPlayerAttributeComponent>(UCAttributeComponentBase::GetAttributes(InstigatorPawn)))
		{
			Attributes->AddSkillPoints(PointsValue);
		}
	}
	Super::ConsumePickup_Implementation(InstigatorPawn);
}

void ACSkillPointsPickup::BeginSuction_Implementation(APawn* InstigatorPawn)
{
	if (bCanSuction)
	{
		Super::BeginSuction_Implementation(InstigatorPawn);
	}
}

void ACSkillPointsPickup::StopSuction_Implementation(APawn* InstigatorPawn)
{
	Super::StopSuction_Implementation(InstigatorPawn);
}