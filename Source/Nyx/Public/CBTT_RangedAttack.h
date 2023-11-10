// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"

#include "CBTT_RangedAttack.generated.h"

/**
 *
 */
UCLASS()
class NYX_API UCBTT_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<AActor> ProjectileClass;
};
