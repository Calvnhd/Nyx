// Copyright (C) 2024 - Calvin Davidson

#include "CAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(
			EnemyBaseBehaviourTree,
			TEXT("EnemyBaseBehaviourTree is nullptr! Please assign EnemyBaseBehaviourTree in your AI Controller")))
	{
		RunBehaviorTree(EnemyBaseBehaviourTree);
	}
}
