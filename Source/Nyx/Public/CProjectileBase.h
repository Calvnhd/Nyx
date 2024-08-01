// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<USphereComponent> SphereComp;
	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	// Aesthetics
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;
	// Explosion effects on hit
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Projectile|Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<URadialForceComponent> ForceComp;

	// OnProjectileHitResponse signature comes from UPrimitiveComponent's FComponentHitSignature OnComponentHit, which
	// SphereComp inherits.
	//
	// We want the projectile (an AActor) to subscribe to hit events that SphereComp broadcasts
	// Hits are blocking (overlaps are not)
	//
	// OnProjectileHitResponse will subscribe to delegate SphereComp->OnComponentHit()
	UFUNCTION(Category = "Nyx|Projectile|Behaviour")
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Projectile|Behaviour")
	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Projectile|Behaviour")
	float DamageAmount;

	FTimerHandle LifetimeTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Projectile|Behaviour")
	float MaximumLifetime;
};
