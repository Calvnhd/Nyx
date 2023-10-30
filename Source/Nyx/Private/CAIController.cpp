// Copyright (C) 2023 - Calvin Davidson

#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(EnemyBehaviourTree,
				   TEXT("Behaviour Tree is nullptr! Please assign EnemyBehaviourTree in your AI Controller")))
	{
		RunBehaviorTree(EnemyBehaviourTree);
	}

	if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
	}
}
