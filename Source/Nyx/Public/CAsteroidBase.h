// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CAsteroidBase.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class URadialForceComponent;

UCLASS()
class NYX_API ACAsteroidBase : public AActor
{
	GENERATED_BODY()

public:
	ACAsteroidBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Asteroid|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|Asteroid|Components")
	TObjectPtr<URadialForceComponent> ForceComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|Asteroid|Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
};
