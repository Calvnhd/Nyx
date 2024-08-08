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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void ActivateSpawnLoop();
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Spawner")
	void SpawnOnce();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void DeactivateSpawnLoop();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void SetLoopLength(float NewLength);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	TSubclassOf<AActor> BallClass;
	FTimerHandle SpawnLoopTimerHandle;
	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	float LoopLength;

	UFUNCTION(BlueprintCallable, Category = "Nyx|Spawner")
	bool IsSpawnerActive() const;

public:
};
