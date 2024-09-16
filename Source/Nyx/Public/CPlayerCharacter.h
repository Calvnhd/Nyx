// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayerCharacter.generated.h"

class USphereComponent;
class UCameraComponent;
class ACSkillPointsPickup;
class USpringArmComponent;
class UCPlayerAttributeComponent;
class UInputMappingContext;
class UCAttributeComponentBase;
class UInputAction;
struct FInputActionValue;

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
	 */

	/* Overrides */

	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/* Components */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Components")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Components")
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Player|Components")
	TObjectPtr<UCPlayerAttributeComponent> PlayerAttributeComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Components")
	TObjectPtr<USphereComponent> PickupSphereComp;

	/* Input */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> AttackPrimaryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> AttackSpecialAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> ShieldAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Player|Input")
	TObjectPtr<UInputAction> CameraLockAction;

	/* Actions */

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	FVector GetCameraTargetLocation() const;
	AActor* GetCameraTargetActor() const;
	FTransform GetCrosshairTargetTM() const;
	FTransform GetLockedTargetTM() const;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Player|Abilities")
	FVector GetMuzzleLocation() const;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float MaxBarrelPitch = 160.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float MinBarrelPitch = 80.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float NeutralBarrelPitch = 90.0f;
	UFUNCTION(BlueprintCallable, Category = "Nyx|Player|Abilities")
	float CalculateBarrelPitch() const;

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass, TObjectPtr<UParticleSystem> MuzzleEffect);

	FTimerHandle AttackPrimaryTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float AttackPrimaryFireRate;
	void AttackPrimary(const FInputActionValue& Value);
	void AttackPrimaryFireOnce();
	void AttackPrimaryBegin();
	void AttackPrimaryEnd();
	void AttackPrimaryResetLoop();
	UPROPERTY(EditAnywhere, Category = "Nyx|Player|Abilities")
	TSubclassOf<AActor> ProjectileClassPrimary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashPrimary;

	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Abilities")
	void CameraLock(const FInputActionValue& Value);
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	bool bCameraIsLocked;
	UPROPERTY(BlueprintReadOnly)
	AActor* CameraLockFocussedEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float CameraLockTraceRadius;

	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Abilities")
	void Dash(const FInputActionValue& Value);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Abilities")
	void OnDashComplete();
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Abilities")
	void Shield(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	float DashStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	float DashTime;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float DashDecelerationPercent;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float DashDecelerationRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	float MaxSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	float EndDashSpeedModifier;
	UFUNCTION(BlueprintCallable, Category = "Nyx|Player|Abilities")
	void ReduceSpeedToMax();
	UFUNCTION(BlueprintCallable, Category = "Nyx|Player|Abilities")
	float GetSpeed() const;
	FTimerHandle ReduceSpeedToMaxTimerHandle;
	FTimerHandle DashTimerHandle;

	/* Events */

	UFUNCTION()
	void NativeHealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
									float NewHealth);
	UFUNCTION()
	void NativeSkillPointsChangedHandler(UCAttributeComponentBase* OwningComp, float Delta, float NewPoints);
	UFUNCTION()
	void NativeCapsuleCompOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										 const FHitResult& SweepResult);
	UFUNCTION()
	void NativePickupSphereOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
										  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
										  const FHitResult& SweepResult);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Events")
	void OnDeath();

	/* Misc */
	void CollectPickup(ACSkillPointsPickup* NewPickup);

	// Exec turns it into a console command on a Character (or Player Controller, GameMode, Cheat Manager)
	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
	/// WIP pickup stuff
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Player|Abilities")
	ACSkillPointsPickup* HeldPickup;
	UPROPERTY()
	TArray<ACSkillPointsPickup*> OrbitingPickups;
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Player|Abilities")
	void AttackSpecial(const FInputActionValue& Value);
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float HeldPickupHeight;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float PickupLaunchImpulseStrength;
};
