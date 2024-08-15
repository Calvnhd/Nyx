// Copyright (C) 2024 - Calvin Davidson

#include "CPickupItemBase.h"

ACPickupItemBase::ACPickupItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;

	bIsSuctionActive = false;
	SuctionForceMultiplier = 1.0f;
}

UStaticMeshComponent* ACPickupItemBase::GetMesh() const
{
	if (MeshComp)
	{
		return MeshComp;
	}
	return nullptr;
}

void ACPickupItemBase::ConsumePickup_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::ConsumePickup_Implementation(InstigatorPawn);
	Destroy();
}

void ACPickupItemBase::BeginSuction_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::BeginSuction_Implementation(InstigatorPawn);

	if (!InstigatorPawn)
	{
		return;
	}
	if (MeshComp)
	{
		PlayerRef = InstigatorPawn;
		bIsSuctionActive = true;
	}
}

void ACPickupItemBase::StopSuction_Implementation(APawn* InstigatorPawn)
{
	ICPickupInterface::StopSuction_Implementation(InstigatorPawn);
	bIsSuctionActive = false;
}

void ACPickupItemBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsSuctionActive && PlayerRef)
	{
		FVector PickupToPlayer = (PlayerRef->GetActorLocation() - GetActorLocation());
		float Distance = PickupToPlayer.Length();
		PickupToPlayer.Normalize();
		MeshComp->AddForce(PickupToPlayer * SuctionForceMultiplier * Distance, NAME_None, true);
	}
}
