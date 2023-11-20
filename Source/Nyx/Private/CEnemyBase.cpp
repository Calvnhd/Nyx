// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"

#include "CAttributeComponent.h"
#include "CCommonDefines.h"
#include "CEnemyAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// this is a useful include to refer to again!
#include "DrawDebugHelpers.h"

// Sets default values
ACEnemyBase::ACEnemyBase()
{
	EnemyAttributeComp = CreateDefaultSubobject<UCEnemyAttributeComponent>("EnemyAttributeComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("EnemySensingComp");
}
void ACEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings
	EnemyAttributeComp->OnEnemyHealthChangedDelegate.AddDynamic(this, &ACEnemyBase::OnHealthChangedResponse);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemyBase::OnCollisionResponse);
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ACEnemyBase::OnPawnSeenResponse);
}
void ACEnemyBase::OnHealthChangedResponse(float Delta, float NewHealth)
{
	if (NewHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enemy destroyed"));
		Destroy();
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


void ACEnemyBase::OnPawnSeenResponse(APawn* Pawn) {
	ACAIController* AIC = Cast<ACAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		// To do, make this an FNAME instead of hard coding
		BBComp->SetValueAsObject("TargetActor", Pawn);
		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}

}

float ACEnemyBase::GetHealthPercent()
{
	return EnemyAttributeComp->GetHealthPercent();
}
