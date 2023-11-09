// Copyright (C) 2023 - Calvin Davidson

#include "CBTS_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <AIModule/Classes/AIController.h>

void UCBTS_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceTo < 2000.0f;

					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange);
				}

			}
		}
	}

}
