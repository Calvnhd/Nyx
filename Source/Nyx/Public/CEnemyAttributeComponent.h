// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CEnemyAttributeComponent.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatorActor,
											 UCEnemyAttributeComponent*, OwningComp, float, Delta, float, NewHealth);

/*
 * Contains and manages the player's EnemyAttributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCEnemyAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyAttributeComponent();

	// Static functions can be called anywhere, without an instance of the class
	UFUNCTION(BlueprintCallable, Category = "Nyx-Attributes")
	static UCEnemyAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx-Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	bool IsAlive();
	void ApplyHealthChange(AActor* InstigatorActor, float Delta);
	float GetHealth();
	float GetHealthMax();
	float GetHealthPercent();
	float GetCollisionDamageAmount();

	UPROPERTY(BlueprintAssignable, Category = "EnemyAttributes")
	FOnHealthChangedSignature OnHealthChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float Health = 1000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyAttributes")
	float CollisionDamageAmount = 100.0f;
};
