// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UCAttributeComponent;

/*
 * The player-controlled character
 */
UCLASS()
class NYX_API ACPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPlayer();

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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;
	// BlueprintReadOnly to use in BP event graph etc, VisibleAnywhere lets me edit in BP details
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UParticleSystem> EngineEffects;

	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TObjectPtr<UParticleSystem> MuzzleFlash;

	UPROPERTY(BlueprintReadWrite)
	bool bIncreasingThrust;
	UPROPERTY(BlueprintReadWrite)
	bool bDecreasingThrust;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
	float AirControlConstant;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float MuzzleHeightOffset;

	// Movement
	UFUNCTION(BlueprintCallable)
	float GetSpeed();
	UFUNCTION(BlueprintCallable)
	void IncrementThrust();
	UFUNCTION(BlueprintCallable)
	void DecrementThrust();
	UFUNCTION(BlueprintCallable)
	float GetThrust();
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const TSubclassOf<AActor> ClassToSpawn);

	// Attack
	UFUNCTION(BlueprintCallable)
	void HandleRotationInput(float InputValue, FVector RotationAxis, float Alpha);
	UFUNCTION(BlueprintCallable)
	FTransform GetCrosshairTargetTM();

	FVector GetMuzzleLocation();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
