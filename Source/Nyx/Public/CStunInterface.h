// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CStunInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCStunInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class NYX_API ICStunInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Nyx|Interface|Stun")
	void Stun(float StunTime);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Nyx|Interface|Stun")
	void Recover();
};
