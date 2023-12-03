// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "CAIController.h"
#include "CEnemyAttributeComponent.h"
#include "CCommonDefines.h"
#include "CEnemyAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
// this is a useful include to refer to again!
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "CWorldUserWidget.h"

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
	EnemyAttributeComp->OnHealthChangedDelegate.AddDynamic(this, &ACEnemyBase::OnHealthChangedResponse);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemyBase::OnCollisionResponse);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACEnemyBase::OnPawnSeenResponse);
}
void ACEnemyBase::OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
										  float NewHealth)
{
	// Damaged
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
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

		//todo -- hitflash. I've just copied this across without testing. Might just work.  Might not.
		//GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

		if (NewHealth <= 0.0f)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enemy killed"));

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
			// Ragdoll is an existing UE template
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
		if (UCEnemyAttributeComponent* PlayerAttributeComp =
				Cast<UCEnemyAttributeComponent>(UCAttributeComponentBase::GetAttributes(OtherActor)))
		{
			PlayerAttributeComp->ApplyHealthChange(this, -EnemyAttributeComp->GetCollisionDamageAmount());
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
