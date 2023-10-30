// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "CAIController.generated.h"

/**
 *
 */
UCLASS()
class NYX_API ACAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* EnemyBehaviourTree;
};
