// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PEnemyBase.generated.h"

class UPAttributeComponentEnemy;

/*
 * Base class for AI controlled enemies
 * Will subclass this into different enemy types
 */
UCLASS()
class PROTO_API APEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	APEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UPAttributeComponentEnemy> AttributeComp;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Not taking any input, so I don't think this matters?
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
