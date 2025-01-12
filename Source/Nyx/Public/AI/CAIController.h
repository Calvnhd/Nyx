// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"

#include "CAIController.generated.h"

class UBehaviorTree;

/**
 *
 */
UCLASS()
class NYX_API ACAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AI")
	UBehaviorTree* EnemyBaseBehaviourTree;
};
