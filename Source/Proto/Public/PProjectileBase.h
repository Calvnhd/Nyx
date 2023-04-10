// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

/*
 * Base projectile class for ranged attacks (both player and AI)
 */
UCLASS()
class PROTO_API APProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	APProjectileBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactVFX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UParticleSystemComponent> EffectComp;

	// Mark as BlueprintCallable as needed
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								 FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void Explode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DamageAmount;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
