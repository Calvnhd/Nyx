// Copyright (C) 2023 - Calvin Davidson

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

	// if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation", PlayerPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}
