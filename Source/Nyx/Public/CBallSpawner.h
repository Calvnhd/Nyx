// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CBallSpawner.generated.h"

UCLASS()
class NYX_API ACBallSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACBallSpawner();

	void ActivateSpawnLoop();
	void SpawnOnce();
	void DeactivateSpawnLoop();
	void SetLoopLength();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	TSubclassOf<AActor> BallClass;
	FTimerHandle SpawnLoopTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	float LoopLength;


public:
};
