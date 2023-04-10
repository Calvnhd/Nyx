// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PAttributeInterface.h"

#include "PCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UPInteractComponent;
class UPAttributeComponentPlayer;

/*
 * The player-controlled character
 */
UCLASS()
class PROTO_API APCharacter : public ACharacter, public IPAttributeInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APCharacter();

protected:
	/*
	 *
	 * EditAnywhere - edit in BP editor and per-instance in level
	 * VisibleAnywhere - 'read-only' in editor and level. (Use for components)
	 * EditDefaultsOnly - hide variable per-instance, edit in BP editor only
	 * VisibleDefaultsOnly - 'read-only' access for variable, only in BP editor (uncommon)
	 * EditInstanceOnly - allow only editing of instance (e.g. when placed in a level)
	 * --
	 * BlueprintReadOnly - read-only in the Blueprint scripting (does not affect 'details' panel)
	 * BlueprintReadWrite - read-write access in Blueprints
	 * --
	 * category = "" = display only for detail panels and blueprint context menu
	 *
	 */

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Components
	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPInteractComponent> InteractComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPAttributeComponentPlayer> AttributeComp;
	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UParticleSystem> CastingEffect;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Interact();

	// Attributes
	virtual void ApplyDamage_Implementation(float DamageAmount) override;

	//UFUNCTION()
	//void OnHealthChangedResponse(float Delta);

	// todo: consider making an AttackComponent or AbilityComponent?
	// ===================================
	// A single simple primary attack
	// Reliable
	// todo: make additional selectable attacks
	// ======== Projectile Attack ========
	// On button pressed
	void AttackPrimary();
	// Timer identifier
	FTimerHandle TimerHandle_AttackPrimary;
	// todo: could a speed modifier on the attribute component be used here?
	// Timer length
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float AttackPrimaryWindUp;
	// Wind up complete
	void AttackPrimary_TimeElapsed();
	// TSubclassOf<> lets us assign some projectile class in BP
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<AActor> ProjectileClassPrimary;
	// Transformation Matrix (Location, rotation, and scale) of the projectile
	FTransform GetProjectileTM();
	// Spawn the projectile
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClassToSpawn);
	// ===================================
	// Special attack
	// Powerful. Expensive. Heavy hitter.
	// todo: make additional selectable attacks
	// ======== Special Attack ========
	// On button pressed
	void AttackSpecial();
	// Timer identifier
	FTimerHandle TimerHandle_AttackSpecial;
	// todo: could a speed modifier on the attribute component be used here?
	// Timer length
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float AttackSpecialWindUp;
	// Wind up complete
	void AttackSpecial_TimeElapsed();
	// TSubclassOf<> lets us assign some projectile class in BP
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<AActor> ProjectileClassSpecial;
	// Call GetProjectileTM() and SpawnProjectile()
	// ===================================
	// Swift directional leap. Very little wind up, slow recovery
	// Powerful and rewarding, but punishable
	// =========== Dash Attack ===========
	// On button pressed
	void AttackDash();
	// Timer identifier
	FTimerHandle TimerHandle_AttackDash;
	// todo: could a speed modifier on the attribute component be used here?
	// todo: consider TObjectPtr<UParticleSystem> CastingEffect
	// todo: a charge animation
	// Timer length
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float AttackDashWindUp;
	// Wind up complete, teleport us some distance forward
	void AttackDash_TimeElapsed();
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float DashDistance;
	// todo: effects, hit box, damage
	// Recovery
	FTimerHandle TimerHandle_AttackDashCooldown;
	float AttackDashCooldown;
	void AttackDashCooldown_TimeElapsed();
	// ===================================
	// 360 degree protection.
	// Little health is lost, take posture damage instead
	// Safe
	// ============== Shield =============
	// On button pressed
	void ShieldUp();
	// Internal flag for checking
	bool IsShielding;
	// Percentage damage is reduced to
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	// todo: change this name -- it's too similar to HealthReduction
	float DamageReduction;
	// Percentage speed is reduced to
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	float SpeedReduction;
	// On button released
	void ShieldDown();
	// todo: effects, does size matter? damage reduction?
	// ===================================

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
