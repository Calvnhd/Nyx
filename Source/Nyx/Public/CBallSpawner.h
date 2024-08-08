// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CBallSpawner.generated.h"

class ACAsteroidBase;

UCLASS()
class NYX_API ACBallSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACBallSpawner();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void ActivateSpawnLoop();

	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Spawner")
	ACAsteroidBase* SpawnOnce();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void DeactivateSpawnLoop();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Spawner")
	void SetLoopLength(float NewLength, bool bHardReset = false);

protected:
	virtual void BeginPlay() override;

	FTimerHandle SpawnLoopTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	TSubclassOf<ACAsteroidBase> BallClass;

	UPROPERTY(EditAnywhere, Category = "Nyx|Spawner")
	float LoopLength;

	UFUNCTION(BlueprintCallable, Category = "Nyx|Spawner")
	bool IsSpawnerActive() const;

	void ExecuteSpawnLoop();

	UFUNCTION(BlueprintImplementableEvent, Category = "Nyx|Spawner")
	FVector GetSpawnLocation();
};
