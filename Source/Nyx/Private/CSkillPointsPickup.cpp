// Copyright (C) 2024 - Calvin Davidson

#include "CSkillPointsPickup.h"

#include "CPlayerAttributeComponent.h"

ACSkillPointsPickup::ACSkillPointsPickup()
{
	PointsValue = 10.0f;
	bCanSuction = true;
	OrbitForceMultiplier = 1.0f;
	OrbitRadiusThreshold = 1000.0f;
}

void ACSkillPointsPickup::SetCanSuction(bool bNewCanSuction)
{
	bCanSuction = bNewCanSuction;
}

void ACSkillPointsPickup::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSuctionActive && PlayerRef)
	{
		FVector PickupToPlayer = (PlayerRef->GetActorLocation() - GetActorLocation());
		float Distance = PickupToPlayer.Length();
		PickupToPlayer.Normalize();
		FVector PerpendicularVector = FRotator(0.0f, 90.0f, 0.0f).RotateVector(PickupToPlayer);

		if (Distance < OrbitRadiusThreshold)
		{
			MeshComp->AddForce(PerpendicularVector * OrbitForceMultiplier * (1 / Distance), NAME_None, true);
		}
	}
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