// Copyright (C) 2023 - Calvin Davidson

#include "PAttributeComponentBase.h"

// Sets default values for this component's properties
UPAttributeComponentBase::UPAttributeComponentBase()
{
	PrimaryComponentTick.bCanEverTick = true;

	Health = 0.0f;
	HealthMax = 1000.0f;
	// HealthQuanta = HealthMax * 0.1;
}

// Called when the game starts
// void UPAttributeComponentBase::BeginPlay()
//{
//	Super::BeginPlay();
//}

// Called every frame
// void UPAttributeComponentBase::TickComponent(float DeltaTime, ELevelTick TickType,
//										 FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

bool UPAttributeComponentBase::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp((Health - Delta), 0, HealthMax);

//	OnHealthChangedTrigger.Broadcast(Delta);

	// Copying this from TLUE... don't (yet) need to return bool. Leave here for now.
	return true;
}

// void UPAttributeComponentBase::ToggleDamageModifier()
//{
//	PostureDamagePercentModifier = 1 - PostureDamagePercentModifier;
//}
