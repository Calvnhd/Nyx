// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidBase.h"

#include "Components/StaticMeshComponent.h"
#include "CAsteroidAttributeComponent.h"

ACAsteroidBase::ACAsteroidBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComp = CreateDefaultSubobject<UCAsteroidAttributeComponent>("AttributeComp");
}

void ACAsteroidBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	AttributeComp->InitializeAttributes();
}

void ACAsteroidBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACAsteroidBase::HealthChangedHandler);
}

void ACAsteroidBase::HealthChangedHandler(AActor* InstigatorActor, UCAsteroidAttributeComponent* OwningComp,
										  float Delta,
									 float NewHealth)
{
	if (NewHealth <= 0.0f)
	{
		Explode();
	}
}

float ACAsteroidBase::GetHealthPercent()
{
	return AttributeComp->GetHealthPercent();
}

void ACAsteroidBase::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
	}
}


void ACAsteroidBase::OnCollisionWithAsteroid() {}

void ACAsteroidBase::OnCollisionWithPlayer() {}

bool ACAsteroidBase::IsAlive()
{
	return AttributeComp->IsAlive();
}

void ACAsteroidBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		Destroy();
	}
}