// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CAsteroidAttributeComponent.h"
#include "CStunInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CAsteroidBase.generated.h"

class UCAttributeComponentBase;
class UCAsteroidAttributeComponent;

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
class NYX_API ACAsteroidBase : public AActor, public ICStunInterface
{
	GENERATED_BODY()

public:
	ACAsteroidBase();

protected:
	/* Overrides */

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Asteroid|Components")
	TObjectPtr<UCAsteroidAttributeComponent> AttributeComp;
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Asteroid|Components")
	UStaticMeshComponent* GetStaticMeshComponent() const;

	/* Events */

	UFUNCTION()
	void NativeHealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta, float NewHealth);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Asteroid|Events")
	void SpawnSmallerAsteroids();
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Asteroid|Events")
	void SpawnItem();
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Asteroid|Events")
	void Explode();

	/* Attributes */

	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Attributes")
	float GetHealthPercent();
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Attributes")
	bool IsAlive();

	/* Classes */

	TSubclassOf<AActor> GetAsteroidClassToSpawn() const;

	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> AsteroidClass_Base;
	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> AsteroidClass_Small;
	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> AsteroidClass_Medium;
	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> AsteroidClass_Large;
	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> AsteroidClass_Largest;
	UPROPERTY(EditAnywhere, Category = "Nyx|Asteroid|Classes")
	TSubclassOf<AActor> ItemDropClass;

	/* Behaviour */

	UPROPERTY(BlueprintReadWrite, Category = "Nyx|Asteroid|Behaviour")
	bool bTickPhysicsHomingForce;
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Behaviour")
	void AddForceInPlayerDirection(AActor* Player);
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Behaviour")
	void EnablePhysicsAndGravity();
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Behaviour")
	void DisablePhysicsAndGravity();

	FTimerHandle StunTimerHandle;
	void OnStunTimerComplete();

	/* Awareness */

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Asteroid|Awareness")
	AActor* GetPlayerRef() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|Asteroid|Awareness")
	FVector GetPlayerDirection(AActor* Player) const;

	/* Interfaces */

	virtual void Stun_Implementation(float StunTime) override;
	virtual void Recover_Implementation() override;

	EAsteroidState RecoverToState;
};
