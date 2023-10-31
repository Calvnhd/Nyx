// Copyright (C) 2023 - Calvin Davidson

#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Kismet/GameplayStatics.h>

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(EnemyBaseBehaviourTree);

	if (APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsObject("TargetActor", MyPawn);
	}
}
