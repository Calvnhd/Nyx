// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidBase.h"

#include "Components/StaticMeshComponent.h"
#include "CAsteroidAttributeComponent.h"

ACAsteroidBase::ACAsteroidBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComp = CreateDefaultSubobject<UCAsteroidAttributeComponent>("AttributeComp");
}

void ACAsteroidBase::OnCollisionWithAsteroid() {}

void ACAsteroidBase::OnCollisionWithPlayer() {}

void ACAsteroidBase::CalculateTraits(EAsteroidSize InSize)
{
	AttributeComp->CalculateAttributes(InSize);
}

//void ACAsteroidBase::OnHealthLost()
//{
//	
//}

void ACAsteroidBase::TakeDamage(int32 DamageTaken) {}

void ACAsteroidBase::IsAlive() {}

void ACAsteroidBase::Death() {}
