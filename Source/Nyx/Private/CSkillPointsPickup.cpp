// Copyright (C) 2024 - Calvin Davidson

#include "CSkillPointsPickup.h"

#include "CPlayerAttributeComponent.h"

ACSkillPointsPickup::ACSkillPointsPickup()
{
	PointsValue = 10.0f;
}

void ACSkillPointsPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACSkillPointsPickup::Pickup_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		if (UCPlayerAttributeComponent* Attributes =
				Cast<UCPlayerAttributeComponent>(UCAttributeComponentBase::GetAttributes(InstigatorPawn)))
		{
			Attributes->AddSkillPoints(PointsValue);
		}
	}
	Super::Pickup_Implementation(InstigatorPawn);
}

void ACSkillPointsPickup::Suction_Implementation(APawn* InstigatorPawn)
{
	Super::Suction_Implementation(InstigatorPawn);
}