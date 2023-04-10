// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PAttributeComponentBase.h"

#include "PAttributeComponentEnemy.generated.h"

/*
 * Contains and manages the attributes for an AI-controlled enemy character
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTO_API UPAttributeComponentEnemy : public UPAttributeComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPAttributeComponentEnemy();

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Poise;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float PoiseMax;

public:
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	// 						   FActorComponentTickFunction* ThisTickFunction) override;
};
