// Copyright (C) 2024 - Calvin Davidson

#include "CPickupItemBase.h"

// Sets default values
ACPickupItemBase::ACPickupItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
}

void ACPickupItemBase::Pickup_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::Pickup_Implementation(InstigatorPawn);
}
