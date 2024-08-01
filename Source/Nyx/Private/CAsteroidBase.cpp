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

void ACAsteroidBase::OnCollisionWithAsteroid() {}

void ACAsteroidBase::OnCollisionWithPlayer() {}

void ACAsteroidBase::TakeDamage(float DamageTaken)
{
	AttributeComp->TakeDamage(DamageTaken);
}

bool ACAsteroidBase::IsAlive()
{
	return AttributeComp->IsAlive();
}