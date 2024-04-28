// Copyright (C) 2023 - Calvin Davidson

#include "CGameModeBase.h"
#include "CEnemyAttributeComponent.h"
#include "CEnemyBase.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ACGameModeBase::ACGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}
void ACGameModeBase::KillAll()
{
	for (TActorIterator<ACEnemyBase> It(GetWorld()); It; ++It)
	{
		ACEnemyBase* Bot = *It;

		UCEnemyAttributeComponent* AttributeComp =
			Cast<UCEnemyAttributeComponent>(UCAttributeComponentBase::GetAttributes(Bot));
		if (Bot->IsAlive())
		{
			// Could also pass in player for kill credit?
			AttributeComp->Kill(this);
		}
	}
}
void ACGameModeBase::StartPlay()
{
	Super::StartPlay();

	// Continuous timer to spawn in more bots
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ACGameModeBase::SpawnBotTimerElapsed,
									SpawnTimerInterval, true);
}
void ACGameModeBase::SpawnBotTimerElapsed()
{
	int32 NumAliveBots = 0;
	// TActorIterator is like a better version of get actors of class
	// It lets us grab any instance of a current class in the current level
	// You can pass in whatever! And it'll return everything derived from that class
	for (TActorIterator<ACEnemyBase> It(GetWorld()); It; ++It)
	{
		ACEnemyBase* Bot = *It;

		/*
			You could also go...
			ACAttributeComp* AttributeComp = ACAttributeComp::GetAttributes(Bot);
			if (ensure(AttributeComp) && ...) ...
		*/
		if (Bot->IsAlive())
		{
			NumAliveBots++;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots"), NumAliveBots);
	const float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		// Expects a time.  Something for X axis.
		DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NumAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity.  Skipping bot spawn."))
		return;
	}

	// This is a bit weird in some ways because it's designed for BP
	UEnvQueryInstanceBlueprintWrapper* QueryInstance =
		UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ACGameModeBase::OnQueryCompletedResponse);
	}
}
void ACGameModeBase::OnQueryCompletedResponse(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
											  EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query failed!"))
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(EnemyClass, Locations[0], FRotator::ZeroRotator);
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
	}
}
