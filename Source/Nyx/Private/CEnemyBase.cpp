// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CAIController.h"
#include "CAttributeComponent.h"
#include "CCommonDefines.h"
#include "CEnemyAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
// this is a useful include to refer to again!
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACEnemyBase::ACEnemyBase()
{
	EnemyAttributeComp = CreateDefaultSubobject<UCEnemyAttributeComponent>("EnemyAttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("EnemySensingComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}
void ACEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings
	EnemyAttributeComp->OnEnemyHealthChangedDelegate.AddDynamic(this, &ACEnemyBase::OnHealthChangedResponse);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemyBase::OnCollisionResponse);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACEnemyBase::OnPawnSeenResponse);
}
void ACEnemyBase::OnHealthChangedResponse(AActor* InstigatorActor, UCEnemyAttributeComponent* OwningComp, float Delta,
										  float NewHealth)
{
	// Damaged
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (NewHealth <= 0.0f)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enemy killed"));

			// stop BT
			// need the AI controller.  It's controlling everything!
			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				// BrainComponent is base class for behaviour tree component
				// Reason in StopLogic is just for debugging
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// Ragdoll
			// Skeletal mesh can simulate physics or use animation data
			// Apply gravity and stuff
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			// Ragdoll should hopefully be a physics preset
			GetMesh()->SetCollisionProfileName("Ragdoll");

			// set lifespan (how long until we call destroy actor on ourselves
			SetLifeSpan(10.0f);
		}
	}
}
void ACEnemyBase::OnCollisionResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (UCAttributeComponent* PlayerAttributeComp =
				Cast<UCAttributeComponent>(OtherActor->GetComponentByClass(UCAttributeComponent::StaticClass())))
		{
			PlayerAttributeComp->ApplyHealthChange(-EnemyAttributeComp->GetCollisionDamageAmount());
		}
	}
}

void ACEnemyBase::OnPawnSeenResponse(APawn* Pawn)
{
	DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	SetTargetActor(Pawn);
}

float ACEnemyBase::GetHealthPercent()
{
	return EnemyAttributeComp->GetHealthPercent();
}

bool ACEnemyBase::IsAlive()
{
	return EnemyAttributeComp->IsAlive();
}

void ACEnemyBase::SetTargetActor(AActor* NewTarget)
{
	if (ACAIController* AIC = Cast<ACAIController>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}
