// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CEnemyAttributeComponent.generated.h"

// Dynamic allows BP assignment
// Multicast allows multiple listeners
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Delta);

/*
 * Contains and manages the enemy's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCEnemyAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyAttributeComponent();

	float GetHealth();
	void ApplyDamage(float Damage);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;
};
