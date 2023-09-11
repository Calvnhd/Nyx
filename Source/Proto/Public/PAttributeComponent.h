// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "PAttributeComponent.generated.h"

// Dynamic allows BP assignment
// Multicast allows multiple listeners
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Delta);

/*
 * Contains and manages the player's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTO_API UPAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPAttributeComponent();

	float GetThrust();
	void IncreaseThrust();
	void DecreaseThrust();
	void SetThrustToMax();
	void SetThrustToMin();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Thrust;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float ThrustMax;
	// Equal to 1% of maximum thrust, such that thrust can always be increased to full capacity in 100 increments
	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float ThrustQuanta;
};
