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

	virtual void Pickup_Implementation(APawn* InstigatorPawn) override;
};
