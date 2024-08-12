// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CPickupItemBase.h"
#include "CoreMinimal.h"

#include "CSkillPointsPickup.generated.h"

/**
 *
 */
UCLASS()
class NYX_API ACSkillPointsPickup : public ACPickupItemBase
{
	GENERATED_BODY()

public:
	ACSkillPointsPickup();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void Pickup_Implementation(APawn* InstigatorPawn) override;
	virtual void Suction_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Item|Value")
	float PointsValue;
};
