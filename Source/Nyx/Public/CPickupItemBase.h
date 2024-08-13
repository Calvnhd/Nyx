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
	UStaticMeshComponent* GetMesh() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Nyx|Item|Behaviour")
	bool bIsSuctionActive;

	virtual void Tick(float DeltaSeconds) override;
	virtual void ConsumePickup_Implementation(APawn* InstigatorPawn) override;
	virtual void BeginSuction_Implementation(APawn* InstigatorPawn) override;
	virtual void StopSuction_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY()
	APawn* PlayerRef;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Item|Behaviour")
	float SuctionForceMultiplier;
};
