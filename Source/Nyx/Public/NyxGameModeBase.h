// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NyxGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class NYX_API ANyxGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANyxGameModeBase();

	// Doesn't have BeginPlay.  It's responsible for calling BeginPlay on everything else!
	virtual void StartPlay() override;
};
