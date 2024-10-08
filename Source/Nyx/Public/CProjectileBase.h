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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|Projectile|Components")
	TObjectPtr<URadialForceComponent> ForceComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx|Projectile|Effects")
	TObjectPtr<UParticleSystemComponent> EffectComp;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Projectile|Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;

	UFUNCTION(Category = "Nyx|Projectile|Events")
	void NativeProjectileHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Projectile|Attributes")
	float DamageAmount;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Projectile|Behaviour")
	void Explode();

	FTimerHandle LifetimeTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Projectile|Behaviour")
	float MaximumLifetime;
};
