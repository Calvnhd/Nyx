// Copyright (C) 2024 - Calvin Davidson

#include "AI/CBTS_CheckHealth.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CAttributeComponentBase.h"

void UCBTS_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Want to check self for current health, and set some kind of flag or something for low health
	// We have easy access to AAIController from which we can get the Pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIController->GetPawn();
	if (UCAttributeComponentBase* AttributeComp = UCAttributeComponentBase::GetAttributes(AIPawn))
	{
		// We have easy access to BlackboardComp
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		// expect this is always set
		if (ensure(BlackboardComp))
		{
			BlackboardComp->SetValueAsBool(HealthKey.SelectedKeyName, (AttributeComp->GetHealth() < AttributeComp->GetHealth() * 0.3));
		}
	}
}
