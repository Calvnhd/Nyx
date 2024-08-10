// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "CPickupItemBase.h"
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

	virtual void Pickup_Implementation(APawn* InstigatorPawn) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Item|Health")
	float PointsAmount;


};
