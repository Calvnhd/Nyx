// Copyright (C) 2024 - Calvin Davidson

#include "CBTT_RangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "CAttributeComponentBase.h"

EBTNodeResult::Type UCBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// This is not valid for your current basic enemy... but might be in the future
		// FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		// This should spawn the projectile somewhere just up above the enemy
		FVector MuzzleLocation = MyPawn->GetMesh()->GetComponentLocation() + FVector(0, 0, 300);

		// Hardcoding TargetActor for the moment still I think?
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		// Fails entire behavior tree loop, early out
		// Not our responsibility to deal with this any further
		if (!UCAttributeComponentBase::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}

		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		// Add some randomness (between some min max) to affect accuracy
		MuzzleRotation.Pitch += FMath::RandRange(0.0f, BulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-BulletSpread, BulletSpread);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator = MyPawn;

		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
