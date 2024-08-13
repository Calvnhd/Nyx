// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "CPickupItemBase.h"

#include "CHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class NYX_API ACHealthPickup : public ACPickupItemBase
{
	GENERATED_BODY()

public:
	ACHealthPickup();

	virtual void ConsumePickup_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Item|Health")
	float HealAmount;
};
