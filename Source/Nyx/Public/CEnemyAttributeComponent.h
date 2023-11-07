// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CEnemyAttributeComponent.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnemyHealthChangedSignature, float, Delta, float, NewHealth);

/*
 * Contains and manages the player's EnemyAttributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCEnemyAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyAttributeComponent();

	bool IsAlive();
	void ApplyHealthChange(float Delta);
	float GetHealth();
	float GetHealthMax();
	float GetHealthPercent();
	float GetCollisionDamageAmount();

	UPROPERTY(BlueprintAssignable, Category = "EnemyAttributes")
	FOnEnemyHealthChangedSignature OnEnemyHealthChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float Health = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float CollisionDamageAmount = 100.0f;
};
