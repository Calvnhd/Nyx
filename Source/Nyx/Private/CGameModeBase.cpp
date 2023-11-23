// Copyright (C) 2023 - Calvin Davidson

#include "CGameModeBase.h"

void ACGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ACGameModeBase::SpawnBotTimerElapsed,
									SpawnTimerInterval, true);
}

ACGameModeBase::ACGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ACGameModeBase::SpawnBotTimerElapsed() {}
