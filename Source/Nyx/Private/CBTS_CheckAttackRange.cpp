// Copyright (C) 2023 - Calvin Davidson

#include "CBTS_CheckAttackRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include <AIModule/Classes/AIController.h>

void UCBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between ai pawn and target actor
	// We get all our info from UBehaviorTreeComponent& OwnerComp
	// We have easy access to BlackboardComp
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	// expect this is always set
	if (ensure(BlackboardComp))
	{
		// currently hardcoding "TargetActor"
		// GetValueAsObject() returns UObject, which does not have actor location
		// This will often be null, so we don't want an ensure
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			// need to get AI Pawn
			// We have easy access to AAIController
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				// which we can get the Pawn from
				APawn* AIPawn = MyController->GetPawn();
				// should also always be true
				if (ensure(AIPawn))
				{
					// finally we can do the distance check
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
					// can expose range to a variable
					bool bWithinRange = DistanceTo < 2000.0f;
					bool bHasLOS = false;
					if (bWithinRange)
					{
						// Checks visibility channel
						bHasLOS = MyController->LineOfSightTo(TargetActor);
					}

					// Blackboard is the memory for AI
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
