// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "BehaviorTree/BTService.h"
#include "CoreMinimal.h"

#include "CBTS_CheckAttackRange.generated.h"

/**
 *
 */
UCLASS()
class NYX_API UCBTS_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	// Handles dropdown keys in BT
	UPROPERTY(EditAnywhere, Category = "Nyx|AI")
	FBlackboardKeySelector AttackRangeKey;

	/** update next tick interval
	 * parent has interval variable
	 * this function should be considered as const (don't modify state of object) if node is not instanced!
	 * bNotifyTick must be set to true for this function to be called
	 * Calling INIT_SERVICE_NODE_NOTIFY_FLAGS in the constructor of the service will set this flag automatically */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
