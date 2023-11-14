// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCAttributeComponent;

UCLASS()
class NYX_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Components
	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx-Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;
	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UParticleSystem> EngineEffects;

	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Nyx-Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Attack")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Attack")
	float MuzzleHeightOffset = 100.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Attack")
	float AutoAimSweepRadius = 800.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Movement")
	float MovementRotationRate = 500.0f;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Movement")
	float TurnRateGamepad = 50.0f;
	void SpawnProjectile();
	UFUNCTION()
	void OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float Delta,
								 float NewHealth);
	UFUNCTION(BlueprintCallable)
	FTransform GetCrosshairTargetTM();

	FVector GetMuzzleLocation();

	bool bIsAutoAimActive = true;

	// Movement
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
