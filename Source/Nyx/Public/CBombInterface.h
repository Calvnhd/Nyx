// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CBombInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCBombInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NYX_API ICBombInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Nyx|Interface|Bomb")
	void Arm();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Nyx|Interface|Bomb")
	void Detonate();
};
