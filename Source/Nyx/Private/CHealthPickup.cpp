// Copyright (C) 2024 - Calvin Davidson

#include "CHealthPickup.h"

#include "CAttributeComponentBase.h"

ACHealthPickup::ACHealthPickup()
{
	HealAmount = 50.0f;
}

void ACHealthPickup::ConsumePickup_Implementation(APawn* InstigatorPawn)
{
	Super::ConsumePickup_Implementation(InstigatorPawn);

	if (InstigatorPawn)
	{
		if (UCAttributeComponentBase* Attributes = UCAttributeComponentBase::GetAttributes(InstigatorPawn))
		{
			Attributes->ApplyHealthChange(this, HealAmount);
		}
	}
}