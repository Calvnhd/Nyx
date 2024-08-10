// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CPickupInterface.generated.h"

// It's made up of two classes. Just some Unreal magic, don't worry about it.
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NYX_API ICPickupInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Pickup(APawn* InstigatorPawn);
};
