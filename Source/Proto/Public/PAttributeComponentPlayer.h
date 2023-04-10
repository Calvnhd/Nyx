// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PAttributeComponentBase.h"

#include "PAttributeComponentPlayer.generated.h"

/*
 * Contains and manages the attributes for a player-controlled character
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTO_API UPAttributeComponentPlayer : public UPAttributeComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPAttributeComponentPlayer();

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float Meter;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float MeterQuanta;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float MeterFull;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// int MeterCount;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// int MeterCountMax;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float MeterMax;

	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float Stamina;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float StaminaMax;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float StaminaQuanta;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float StaminaRecoverIncrement;

	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float Posture;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float PostureMax;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float PostureQuanta;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float PostureDamagePercentModifier;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float PostureRecoverIncrement;

public:
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//						   FActorComponentTickFunction* ThisTickFunction) override;
};
