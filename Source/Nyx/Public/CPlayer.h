// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CAttributeInterface.h"
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
class NYX_API ACPlayer : public APawn, public ICAttributeInterface
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
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;

	// Want BlueprintReadOnly so we can use it in BP. Going to add all our physics to this component.
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	// UPROPERTY(EditDefaultsOnly, Category = "Effects")
	// TObjectPtr<UParticleSystem> EngineEffects;

	UPROPERTY(BlueprintReadWrite)
	bool bIncreasingThrust;
	UPROPERTY(BlueprintReadWrite)
	bool bDecreasingThrust;

	// Movement
	UFUNCTION(BlueprintCallable)
	float GetThrust();
	UFUNCTION(BlueprintCallable)
	void IncrementThrust();
	UFUNCTION(BlueprintCallable)
	void DecrementThrust();
	// void YawRight();
	// void YawLeft();
	// void PitchUp();
	// void PitchDown();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
