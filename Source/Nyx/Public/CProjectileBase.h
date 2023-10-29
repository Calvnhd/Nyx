// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

/*
 * Base projectile class for ranged attacks (both player and AI)
 */
// ABSTRACT marks as incomplete, keeping this out of certain dropdown windows like SpawnActor in Unreal
UCLASS(ABSTRACT)
class NYX_API ACProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ACProjectileBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	// Root component for collisions (and everything else)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;
	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	// Aesthetics
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;
	// Explosion effects on hit
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;

	// OnProjectileHitResponse signature comes from UPrimitiveComponent, which SphereComp inherits.
	// FComponentHitSignature OnComponentHit
	//
	// We want the projectile (an AActor) to subscribe to hit events that SphereComp broadcasts
	// Hits are blocking (overlaps are not)
	//
	// OnProjectileHitResponse will subscribe to delegate SphereComp->OnComponentHit()
	UFUNCTION()
	virtual void OnProjectileHitResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;
};
