// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CPickupItemBase.h"
#include "CoreMinimal.h"

#include "CSkillPointsPickup.generated.h"

class UStaticMeshComponent;

/**
 *
 */
UCLASS()
class NYX_API ACSkillPointsPickup : public ACPickupItemBase
{
	GENERATED_BODY()

public:
	ACSkillPointsPickup();
	void SetCanSuction(bool bNewCanSuction);

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void ConsumePickup_Implementation(APawn* InstigatorPawn) override;
	virtual void BeginSuction_Implementation(APawn* InstigatorPawn) override;
	virtual void StopSuction_Implementation(APawn* InstigatorPawn) override;

	bool bCanSuction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|SkillPoints")
	float PointsValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float OrbitForceMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float RepelForceMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float OrbitRadiusThreshold;
};
