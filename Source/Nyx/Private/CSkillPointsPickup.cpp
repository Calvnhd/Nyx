// Copyright (C) 2024 - Calvin Davidson

#include "CSkillPointsPickup.h"

#include "CPlayerAttributeComponent.h"

ACSkillPointsPickup::ACSkillPointsPickup()
{
	PointsAmount = 10.0f;
}

void ACSkillPointsPickup::Pickup_Implementation(APawn* InstigatorPawn)
{
	Super::Pickup_Implementation(InstigatorPawn);
	if (InstigatorPawn)
	{
		// todo: make UCPlayerAttributeComponent so you don't have to cast?
		if (UCPlayerAttributeComponent* Attributes =
				Cast<UCPlayerAttributeComponent>(UCAttributeComponentBase::GetAttributes(InstigatorPawn)))
		{
			// Add skill points
		}
	}
}