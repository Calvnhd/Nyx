// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CTargetingComponent.generated.h"

class UCameraComponent;
class UStaticMeshComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCTargetingComponent();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float FindTargetTraceRadius;

public:
	// A Transformation Matrix at the muzzle, looking at the crosshair target
	FTransform GetCrosshairTargetTM(UCameraComponent* Camera, FVector MuzzleLocation) const;
	// A Transformation Matrix at the muzzle, looking at the specified target
	FTransform GetLockedTargetTM(FVector LockedTargetLocation, FVector MuzzleLocation) const;
	// Get location of whatever the crosshair is targeting
	FVector GetCrosshairTargetLocation(UCameraComponent* Camera, FVector MuzzleLocation) const;
	// Find a new target to focus
	AActor* FindTargetActor(UCameraComponent* Camera, FVector MuzzleLocation) const;
};
