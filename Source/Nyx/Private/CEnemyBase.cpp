// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"
#include "CCommonDefines.h"
#include "CEnemyAttributeComponent.h"
#include <Components/CapsuleComponent.h>

// Sets default values
ACEnemyBase::ACEnemyBase()
{
	// PrimaryActorTick.bCanEverTick = true;
	EnemyAttributeComp = CreateDefaultSubobject<UCEnemyAttributeComponent>("EnemyAttributeComp");
}
void ACEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}
void ACEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	EnemyAttributeComp->OnEnemyHealthChangedDelegate.AddDynamic(this, &ACEnemyBase::OnHealthChangedResponse);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACEnemyBase::OnCollisionResponse);
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
{}
float ACEnemyBase::GetHealthPercent()
{
	return EnemyAttributeComp->GetHealthPercent();
}
