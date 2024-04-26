// Copyright (C) 2023 - Calvin Davidson

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
	 * category = "" = display only for detail panels and blueprint context menu
	 *
	 */

	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	/* Components */
	
	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(VisibleAnywhere, Category = "Nyx-Components")
	TObjectPtr<UCPlayerAttributeComponent> PlayerAttributeComp;

	/* Abilities */

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void AttackPrimary();

	/* Other stuff you need to re-organise */
	
	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Nyx-Abilities")
	TSubclassOf<AActor> ProjectileClassPrimary;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Abilities")
	TObjectPtr<UParticleSystem> MuzzleFlashPrimary;

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);

	UFUNCTION(BlueprintCallable)
	FTransform GetCrosshairTargetTM();

	FVector GetMuzzleLocation();
	
	UFUNCTION()
	void OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
								 float NewHealth);

	// Exec turns it into a console command on a Character (or Player Controller, GameMode, Cheat Manager)
	UFUNCTION(Exec)
	void HealSelf(float Amount = 1000);
};
