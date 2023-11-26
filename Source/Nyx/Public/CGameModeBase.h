// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "CGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

/**
 * Think of this as the game's rules
 */
UCLASS()
class NYX_API ACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-AI")
	float SpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-AI")
	TSubclassOf<AActor> EnemyClass;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompletedResponse(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-AI")
	UCurveFloat* DifficultyCurve;

public:
	// Doesn't BeginPlay(). Responsible for calling BeginPlay()!
	virtual void StartPlay() override;

	ACGameModeBase();

};
