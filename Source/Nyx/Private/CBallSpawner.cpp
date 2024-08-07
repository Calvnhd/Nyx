// Copyright (C) 2024 - Calvin Davidson


#include "CBallSpawner.h"

// Sets default values
ACBallSpawner::ACBallSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACBallSpawner::ActivateSpawnLoop()
{
	GetWorldTimerManager().SetTimer(SpawnLoopTimerHandle, this, &ACBallSpawner::SpawnOnce, LoopLength, true, 0);
}

void ACBallSpawner::SpawnOnce()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(BallClass, GetActorTransform(), SpawnParams);
}

// Called when the game starts or when spawned
void ACBallSpawner::BeginPlay()
{
	Super::BeginPlay();
	ActivateSpawnLoop();
}


