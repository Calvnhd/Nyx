// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "CAttributeComponentBase.h"

#include "CPlayerAttributeComponent.generated.h"

/*
 * Contains and manages the player's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCPlayerAttributeComponent : public UCAttributeComponentBase
{
	GENERATED_BODY()

public:
	UCPlayerAttributeComponent();
};
