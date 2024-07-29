// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCPlayerAttributeComponent;
class UInputMappingContext;
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

	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	/* Components */

	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Components")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx|Components")
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Components")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Components")
	TObjectPtr<UCPlayerAttributeComponent> PlayerAttributeComp;

	/* Input Actions */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> AttackPrimaryAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> AttackSpecialAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx|Input")
	TObjectPtr<UInputAction> ShieldAction;

	/* Actions */

	void DoNothing();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	FVector GetCameraTargetLocation() const;
	FTransform GetCrosshairTargetTM() const;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Nyx|Abilities")
	FVector GetMuzzleLocation() const;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Abilities")
	float MaxBarrelPitch = 160.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Abilities")
	float MinBarrelPitch = 80.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Abilities")
	float NeutralBarrelPitch = 90.0f;
	UFUNCTION(BlueprintCallable, Category = "Nyx|Abilities")
	float CalculateBarrelPitch() const;

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);
	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass, TObjectPtr<UParticleSystem> MuzzleEffect);

	FTimerHandle AttackPrimaryTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Abilities")
	float AttackPrimaryFireRate = 1.0f;
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Abilities")
	void AttackPrimary(const FInputActionValue& Value);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Abilities")
	void AttackPrimaryFireOnce();
	void AttackPrimaryBegin();
	void AttackPrimaryEnd();
	void AttackPrimaryResetLoop();
	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Nyx|Abilities")
	TSubclassOf<AActor> ProjectileClassPrimary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashPrimary;

	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Abilities")
	void AttackSpecial(const FInputActionValue& Value);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Abilities")
	void Dash(const FInputActionValue& Value);
	UFUNCTION(BlueprintNativeEvent, Category = "Nyx|Abilities")
	void Shield(const FInputActionValue& Value);

	/* Attributes */

	UFUNCTION()
	void OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
								 float NewHealth);

	// Exec turns it into a console command on a Character (or Player Controller, GameMode, Cheat Manager)
	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
};
