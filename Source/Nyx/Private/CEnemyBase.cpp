// Copyright (C) 2024 - Calvin Davidson

#include "CEnemyBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "CAIController.h"
#include "CCommonDefines.h"
#include "CEnemyAttributeComponent.h"
#include "CWorldUserWidget.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h" // this is a useful include to refer to again!
#include "Perception/PawnSensingComponent.h"

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
	EnemyAttributeComp->OnHealthChanged.AddDynamic(this, &ACEnemyBase::HealthChangedHandler);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemyBase::CollisionHandler);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACEnemyBase::PawnSeenHandler);
}
void ACEnemyBase::HealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
										  float NewHealth)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this && InstigatorActor != nullptr)
		{
			SetTargetActor(InstigatorActor);
		}
		if (ActiveHealthBar == nullptr)
		{
			// CreateWidget is available anywhere
			// Owning object is expected to be the player (i.e. something related to the UI)
			ActiveHealthBar = CreateWidget<UCWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar)
			{
				// need to add this before adding to viewport, because EventConstruct is called in BP like straight away
				ActiveHealthBar->AttachedActor = this;
				// Don't care about Z order for now
				ActiveHealthBar->AddToViewport();
			}
		}
		if (NewHealth <= 0.0f)
		{
			// need the AI controller.  It's controlling everything!
			if (AAIController* AIC = Cast<AAIController>(GetController()))
			{
				// BrainComponent is base class for behaviour tree component
				// Reason passed to StopLogic is just for debugging
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// Skeletal mesh can simulate physics or use animation data
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			// Ragdoll is an existing UE template
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// set lifespan (how long until we call destroy actor on ourselves)
			SetLifeSpan(10.0f);
		}
	}
}
void ACEnemyBase::CollisionHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor,
									  UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (UCAttributeComponentBase* PlayerAttributeComp = UCAttributeComponentBase::GetAttributes(OtherActor))
		{
			PlayerAttributeComp->ApplyHealthChange(this, -EnemyAttributeComp->GetCollisionDamageAmount());
		}
	}
}
void ACEnemyBase::PawnSeenHandler(APawn* Pawn)
{
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
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
