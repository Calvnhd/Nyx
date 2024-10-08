// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CBombInterface.h"
#include "CPickupItemBase.h"
#include "CoreMinimal.h"

#include "CSkillPointsPickup.generated.h"

class USphereComponent;

/**
 *
 */
UCLASS()
class NYX_API ACSkillPointsPickup : public ACPickupItemBase, public ICBombInterface
{
	GENERATED_BODY()

public:
	ACSkillPointsPickup();
	void SetCanSuction(bool bNewCanSuction);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Pickup|Components")
	TObjectPtr<USphereComponent> BlastSphere;

	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	// Pickup Interface
	virtual void ConsumePickup_Implementation(APawn* InstigatorPawn) override;
	virtual void BeginSuction_Implementation(APawn* InstigatorPawn) override;
	virtual void StopSuction_Implementation(APawn* InstigatorPawn) override;
	// Bomb Interface
	virtual void Arm_Implementation() override;
	virtual void Detonate_Implementation() override;
	UFUNCTION()
	void NativeComponentHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor,
								   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Nyx|Pickup|Behaviour")
	void BlastDamageAndStun();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Damage")
	float BlastDamageAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Damage")
	float CollisionDamageAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Damage")
	float StunTimeAmount;

	bool bIsArmed;
	bool bCanSuction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|SkillPoints")
	float PointsValue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float OrbitForceMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float RepelForceMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Pickup|Behaviour")
	float OrbitRadiusThreshold;
};
