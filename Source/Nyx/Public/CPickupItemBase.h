// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "CPickupInterface.h"
#include "GameFramework/Actor.h"

#include "CPickupItemBase.generated.h"

UCLASS()
class NYX_API ACPickupItemBase : public AActor, public ICPickupInterface
{
	GENERATED_BODY()
	
public:	
	ACPickupItemBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Nyx|Item|Behaviour")
	bool bSuction;

	virtual void Tick(float DeltaSeconds) override;
	virtual void Pickup_Implementation(APawn* InstigatorPawn) override;
	virtual void Suction_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY()
	APawn* PlayerRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Item|Behaviour")
	float SuctionForceMultiplier;
};
