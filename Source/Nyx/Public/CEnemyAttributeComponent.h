// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "CAttributeComponentBase.h"

#include "CEnemyAttributeComponent.generated.h"

/*
 * Contains and manages the player's EnemyAttributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCEnemyAttributeComponent : public UCAttributeComponentBase
{
	GENERATED_BODY()

public:
	UCEnemyAttributeComponent();

	float GetCollisionDamageAmount();

	UFUNCTION(BlueprintCallable, Category = "Nyx|EnemyAttributes")
	bool Kill(AActor* InstigatorActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|EnemyAttributes")
	float CollisionDamageAmount = 50.0f;
};
