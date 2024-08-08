// Copyright (C) 2024 - Calvin Davidson

#include "CBallSpawner.h"

// Sets default values
ACBallSpawner::ACBallSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	LoopLength = 5.0f;
}

void ACBallSpawner::ActivateSpawnLoop_Implementation()
{
	GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &ACBallSpawner::SpawnOnce, LoopLength, true, 0);
}

void ACBallSpawner::SpawnOnce_Implementation()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BallClass, GetActorTransform(), SpawnParams);
}

void ACBallSpawner::DeactivateSpawnLoop_Implementation()
{
	GetWorldTimerManager().ClearTimer(SpawnLoopTimerHandle);
}

void ACBallSpawner::SetLoopLength_Implementation(float NewLength)
{
	LoopLength = NewLength;
	if (IsSpawnerActive)
	{
		float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(SpawnLoopTimerHandle);
		GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &ACBallSpawner::ActivateSpawnLoop, LoopLength,
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
