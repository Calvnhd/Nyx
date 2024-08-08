// Copyright (C) 2024 - Calvin Davidson

#include "CBallSpawner.h"

#include "CAsteroidBase.h"

// Sets default values
ACBallSpawner::ACBallSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	LoopLength = 5.0f;
}

void ACBallSpawner::ActivateSpawnLoop_Implementation()
{
	GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &ACBallSpawner::ExecuteSpawnLoop, LoopLength, true, 0);
}
void ACBallSpawner::ExecuteSpawnLoop()
{
	SpawnOnce();
}
ACAsteroidBase* ACBallSpawner::SpawnOnce_Implementation()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (ACAsteroidBase* SpawnedAsteroid =
			GetWorld()->SpawnActor<ACAsteroidBase>(BallClass, FTransform(GetActorRotation(), GetSpawnLocation()), SpawnParams))
	{
		return SpawnedAsteroid;
	}
	return nullptr;
}

void ACBallSpawner::DeactivateSpawnLoop_Implementation()
{
	GetWorldTimerManager().ClearTimer(SpawnLoopTimerHandle);
}

void ACBallSpawner::SetLoopLength_Implementation(float NewLength, bool bHardReset)
{
	LoopLength = NewLength;
	if (!IsSpawnerActive())
	{
		return;
	}
	if (bHardReset)
	{
		ActivateSpawnLoop();
	}
	else
	{
		// needs testing
		float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(SpawnLoopTimerHandle);
		GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &ACBallSpawner::ActivateSpawnLoop, TimeRemaining,
										false);
	}
}

void ACBallSpawner::BeginPlay()
{
	Super::BeginPlay();
	ActivateSpawnLoop();
}

bool ACBallSpawner::IsSpawnerActive() const
{
	return GetWorldTimerManager().IsTimerActive(SpawnLoopTimerHandle);
}


