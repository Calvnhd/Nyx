// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CAsteroidAttributeComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class UCAttributeComponentBase;
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
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Asteroid|Components")
	TObjectPtr<UCAsteroidAttributeComponent> AttributeComp;

	// Events
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Asteroid|Events")
	void OnHealthChanged(AActor* InstigatorActor, UCAsteroidAttributeComponent* OwningComp, float Delta,
						 float NewHealth);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Asteroid|Events")
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
					 FVector NormalImpulse, const FHitResult& Hit);
	void OnCollisionWithAsteroid();
	void OnCollisionWithPlayer();
	void SpawnSmallerAsteroids();
	void SpawnItem();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Asteroid|Events")
	void Explode();

	// Attributes
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Attributes")
	float GetHealthPercent();
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Attributes")
	bool IsAlive();
};
