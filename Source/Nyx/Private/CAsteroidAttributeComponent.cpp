// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidAttributeComponent.h"

UCAsteroidAttributeComponent::UCAsteroidAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UCAsteroidAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCAsteroidAttributeComponent::PostInitProperties()
{
	Super::PostInitProperties();
}

// Called every frame
void UCAsteroidAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType,
												 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCAsteroidAttributeComponent::CalculateAttributes(EAsteroidSize InSize)
{
	Size = InSize;
	switch (Size)
	{
		case EAsteroidSize::Base:
			SizeMultiplier = 1;
		case EAsteroidSize::Small:
			SizeMultiplier = 2;
		case EAsteroidSize::Medium:
			SizeMultiplier = 5;
		case EAsteroidSize::Large:
			SizeMultiplier = 10;
		case EAsteroidSize::Largest:
			SizeMultiplier = 20;
		default:
			SizeMultiplier = 1;
	}
	Health = BaseHealth * SizeMultiplier;
	Power = BasePower * SizeMultiplier;
}
