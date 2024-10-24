// Copyright (C) 2024 - Calvin Davidson

#include "AI/CBTS_CheckHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// We get all our info from input param UBehaviorTreeComponent& OwnerComp
	// We have easy access to BlackboardComp
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// GetValueAsObject() returns UObject, which does not have actor location
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	// need to get AI Pawn
	// We have easy access to AAIController
	AAIController* AIController = OwnerComp.GetAIOwner();
	// which we can get the Pawn from
	APawn* AIPawn = AIController->GetPawn();
	// finally we can do the distance check
	float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	bool bHasLOS = false;
	// Checks visibility channel
	bHasLOS = AIController->LineOfSightTo(TargetActor);
	// Blackboard is the memory for AI
	BlackboardComp->SetValueAsBool(HealthKey.SelectedKeyName, (bHasLOS));
}
