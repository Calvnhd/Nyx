// Copyright (C) 2024 - Calvin Davidson

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

	//--- begin TL stuff I think?
	FTimerHandle TimerHandle_SpawnBots;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|GameMode|AI")
	float SpawnTimerInterval;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|GameMode|AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|GameMode|AI")
	TSubclassOf<AActor> EnemyClass;
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|GameMode|Events")
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void OnQueryCompletedResponse(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|GameMode|AI")
	UCurveFloat* DifficultyCurve;
	//--- end TL


public:
	// Doesn't BeginPlay(). Responsible for calling BeginPlay()!
	virtual void StartPlay() override;

	ACGameModeBase();

	UFUNCTION(Exec)
	void KillAll();


};
