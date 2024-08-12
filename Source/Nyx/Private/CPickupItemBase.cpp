// Copyright (C) 2024 - Calvin Davidson

#include "CPickupItemBase.h"

// Sets default values
ACPickupItemBase::ACPickupItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	bSuction = false;
	SuctionForceMultiplier = 1.0f;
}

void ACPickupItemBase::Pickup_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::Pickup_Implementation(InstigatorPawn);
	Destroy();
}

void ACPickupItemBase::Suction_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::Suction_Implementation(InstigatorPawn);

	if (!InstigatorPawn)
	{
		return;
	}
	if (MeshComp)
	{
		PlayerRef = InstigatorPawn;
		bSuction = true;
	}
}

void ACPickupItemBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bSuction && PlayerRef)
	{
		MeshComp->AddForce((PlayerRef->GetActorLocation() - GetActorLocation()) * SuctionForceMultiplier, NAME_None, true);
	}
}
