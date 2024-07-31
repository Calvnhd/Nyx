// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "CAsteroidAttributeComponent.h"
#include "GameFramework/Actor.h"

class UCAsteroidAttributeComponent;

#include "CAsteroidBase.generated.h"

/*

The super basic enemy type in my super basic Asteroids clone!

A simple bouncing ball.
No intelligence.
Perpetually bounces across the map.
Varies in size (start with three -- small, medium, large)
On collision with other balls
	- large balls bounce off each other
	- smaller combinations absorb each other, increasing proportional size and other traits
Collision with ship - effect is proportional to size
	- Knock back
	- Damage
	- Stun?
Collision with ship weapon
	- takes damage
	- on damage lost threshold, break into some number of lower sized balls
	- smallest ball has some percentage chance to drop an item
*/


UCLASS()
class NYX_API ACAsteroidBase : public AActor
{
	GENERATED_BODY()

public:
	ACAsteroidBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Components")
	TObjectPtr<UCAsteroidAttributeComponent> AttributeComp;

	void OnCollisionWithAsteroid();
	void OnCollisionWithPlayer();
	UFUNCTION(BlueprintCallable, Category = "Nyx|Attributes")

	void CalculateTraits(EAsteroidSize InSize);
	//void OnHealthLost();
	void TakeDamage(int32 DamageTaken);
	void IsAlive();
	void Death();

};
