// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStaticMeshComponent;
class UCAttributeComponent;

// The player-controlled character

UCLASS()
class NYX_API ACPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	// Epic recommends TObjectPtr over raw pointers in header files with UPROPERTY for UE5
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<UCameraComponent> CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Nyx-Components")
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nyx-Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;

	// TSubclassOf<> lets us assign some class in editor and edit it wherever
	UPROPERTY(EditAnywhere, Category = "Nyx-Attack")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx-Attack")
	TObjectPtr<UParticleSystem> MuzzleFlash;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
