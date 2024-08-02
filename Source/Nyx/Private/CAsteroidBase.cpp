// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidBase.h"

#include "CAsteroidAttributeComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACAsteroidBase::ACAsteroidBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AttributeComp = CreateDefaultSubobject<UCAsteroidAttributeComponent>("AttributeComp");
}

void ACAsteroidBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	AttributeComp->InitializeAttributes();
}

void ACAsteroidBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACAsteroidBase::OnHealthChanged);
}

void ACAsteroidBase::OnHealthChanged_Implementation(AActor* InstigatorActor, UCAsteroidAttributeComponent* OwningComp,
													float Delta, float NewHealth)
{
	if (NewHealth <= 0.0f)
	{
		Explode();
	}
}

float ACAsteroidBase::GetHealthPercent()
{
	return AttributeComp->GetHealthPercent();
}

void ACAsteroidBase::OnCollision_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
												UPrimitiveComponent* OtherComp, FVector NormalImpulse,
												const FHitResult& Hit)
{}

void ACAsteroidBase::OnCollisionWithAsteroid() {}

void ACAsteroidBase::OnCollisionWithPlayer() {}

bool ACAsteroidBase::IsAlive()
{
	return AttributeComp->IsAlive();
}

void ACAsteroidBase::SpawnSmallerAsteroids()
{
	TSubclassOf<AActor> AsteroidClass = GetAsteroidClassToSpawn();
	if (!ensureAlways(AsteroidClass))
	{
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTM(UKismetMathLibrary::RandomRotator(), GetActorLocation());
	for (int i = 0; i < AttributeComp->GetNumberOfAsteroidsToSpawn(); ++i)
	{
		GetWorld()->SpawnActor<AActor>(AsteroidClass, SpawnTM, SpawnParams);
	}
}

TSubclassOf<AActor> ACAsteroidBase::GetAsteroidClassToSpawn() const
{
	switch (AttributeComp->GetSize())
	{
		case EAsteroidSize::Base:
			return {};
		case EAsteroidSize::Small:
			return AsteroidClass_Base;
		case EAsteroidSize::Medium:
			return AsteroidClass_Small;
		case EAsteroidSize::Large:
			return AsteroidClass_Medium;
		case EAsteroidSize::Largest:
			return AsteroidClass_Large;
		default:
			return {};
	}
}
void ACAsteroidBase::SpawnItem() {}

void ACAsteroidBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		if (AttributeComp->GetSize() != EAsteroidSize::Base)
		{
			SpawnSmallerAsteroids();
		}
		else
		{
			SpawnItem();
		}
		Destroy();
	}
}