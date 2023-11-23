// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CGameModeBase.generated.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

/**
 *
 */
UCLASS()
class NYX_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-AI")
	float SpawnTimerInterval;

public:

	// Doesn't BeginPlay(). Responsible for calling BeginPlay()!
	virtual void StartPlay() override;

	ACGameModeBase();

	void SpawnBotTimerElapsed();
};
