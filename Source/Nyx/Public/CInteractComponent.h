// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CInteractComponent.generated.h"

// Notice that boilerplate UActorComponents are structurally similar to Actors
// UActorComponents are good for code separation and reuseability
// Player character will have an interact function, but that will just call this interact component.  That way the
// actual mechanics for interacting can be reused.

/*
 * Gives a character the ability to interact
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCInteractComponent();
//	void Interact();
//
//protected:
//	virtual void BeginPlay() override;
//
//public:
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
