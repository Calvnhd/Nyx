// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCPlayerAttributeComponent;

UCLASS()
class NYX_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayerCharacter();

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

	virtual void PostInitializeComponents() override;

	/* Components */
	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Nyx-Components")
	TObjectPtr<UCPlayerAttributeComponent> PlayerAttributeComp;

	/* Abilities */
	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Nyx-Abilities")
	TSubclassOf<AActor> ProjectileClassPrimary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashPrimary;
	UPROPERTY(EditAnywhere, Category = "Nyx-Abilities")
	TSubclassOf<AActor> ProjectileClassSecondary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashSecondary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	TObjectPtr<UParticleSystem> ShieldEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	float MuzzleHeightOffset = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	float AutoAimSweepRadius = 800.0f;
	void AttackPrimary();
	void AttackSecondary();
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);
	UFUNCTION(BlueprintCallable)
	FTransform GetCrosshairTargetTM();
	FVector GetMuzzleLocation();
	bool bIsAutoAimActive = true;

	/* Movement */
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Movement")
	float MovementRotationRate = 500.0f;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Movement")
	float TurnRateGamepad = 50.0f;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UFUNCTION()
	void OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
								 float NewHealth);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Exec turns it into a console command on a Character (or Player Controller, GameMode, Cheat Manager)
	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
};
