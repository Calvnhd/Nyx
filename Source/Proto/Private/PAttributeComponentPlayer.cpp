// Copyright (C) 2023 - Calvin Davidson

#include "PAttributeComponentPlayer.h"

// Sets default values for this component's properties
UPAttributeComponentPlayer::UPAttributeComponentPlayer()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Meter = 0.0f;
	// MeterFull = 1000.0f;
	// MeterQuanta = MeterFull * 0.05;
	// MeterCount = 0;
	// MeterCountMax = 3;
	// MeterMax = MeterCountMax * MeterFull;

	// Stamina = 0.0f;
	// StaminaMax = 1000.0f;
	// StaminaQuanta = StaminaMax * 0.01;
	//// Update and implement later
	// StaminaRecoverIncrement = 0.0f;

	// Posture = 0.0f;
	// PostureMax = 1000.0f;
	//// Damage to posture as a percent of total damage taken (no shield).  Remainder damage taken as health damage.
	//// Inverse health and posture damage when shielding
	// PostureDamagePercentModifier = 0.05f;
	//// Update and implement later
	// PostureRecoverIncrement = 0.0f;
}

// Called when the game starts
// void UPAttributeComponentPlayer::BeginPlay()
//{
//	Super::BeginPlay();
//}

// Called every frame
// void UPAttributeComponentPlayer::TickComponent(float DeltaTime, ELevelTick TickType,
//										 FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

// void UPAttributeComponentPlayer::ToggleDamageModifier()
//{
//	PostureDamagePercentModifier = 1 - PostureDamagePercentModifier;
// }
