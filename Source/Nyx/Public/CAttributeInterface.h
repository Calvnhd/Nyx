// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CAttributeInterface.generated.h"

// Interfaces are a shared API without specifying what the actual implementation does yet
// You can make generic e.g. 'Interact' functions whose behaviour will change depending on the context of the
// interaction

// It's in two classes. Just some Unreal magic, don't worry about it.
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

/*
 * A shared API for generic functions whose behaviour can be implemented by another class and change depending on the
 * context
 *
 * e.g. Interact -- Implement this interface for any Actor that can be interacted with.  The instigator can call a
 * generic interact function, but the result will change depending on the object. An item could be picked up, a door
 * could be opened etc.
 */
class NYX_API ICAttributeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface
public:
	// InstigatorPawn allows you to pass on a character that is doing the interaction
	// BlueprintNativeEvent means code expects a C++ implementation, but we can still implement it in BP if we want
	// I think we omit the virtual keyword because BlueprintNativeEvent indicates that it's designed to be overridden.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyDamage(float DamageAmount);
};
