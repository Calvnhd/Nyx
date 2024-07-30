// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidBase.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACAsteroidBase::ACAsteroidBase()
{
	PrimaryActorTick.bCanEverTick = false;
}
