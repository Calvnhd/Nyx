// Copyright (C) 2023 - Calvin Davidson

#include "PAttributeComponentEnemy.h"

// Sets default values for this component's properties
UPAttributeComponentEnemy::UPAttributeComponentEnemy()
{
	PrimaryComponentTick.bCanEverTick = true;

	PoiseMax = 100;
	Poise = PoiseMax;
}

// Called when the game starts
// void UPAttributeComponentEnemy::BeginPlay()
//{
//	Super::BeginPlay();
//}

// Called every frame
// void UPAttributeComponentEnemy::TickComponent(float DeltaTime, ELevelTick TickType,
//											  FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

// void UPAttributeComponentEnemy::ToggleDamageModifier()
//{
//	PostureDamagePercentModifier = 1 - PostureDamagePercentModifier;
//}
