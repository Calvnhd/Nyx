// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAttributeComponent.generated.h"

// Dynamic allows BP assignment
// Multicast allows multiple listeners
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Delta);

/*
 * Contains and manages the player's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponent();

	float GetSpeed();
	float GetThrust();
	void IncrementThrust();
	void DecrementThrust();
	bool IsAlive();
	void ApplyHealthChange(float Delta);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float ThrustPercent;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float ThrustPercentMax;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float SpeedMax;
};
