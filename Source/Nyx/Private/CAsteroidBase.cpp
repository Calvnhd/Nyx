// Copyright (C) 2024 - Calvin Davidson

#include "CAsteroidBase.h"

#include "CAsteroidAttributeComponent.h"
#include "CAttributeComponentBase.h"
#include "Kismet/KismetMathLibrary.h"

ACAsteroidBase::ACAsteroidBase()
{
	PrimaryActorTick.bCanEverTick = true;
	AttributeComp = CreateDefaultSubobject<UCAsteroidAttributeComponent>("AttributeComp");

	bTickPhysicsHomingForce = false;
}

void ACAsteroidBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->InitializeAttributes();
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACAsteroidBase::OnHealthChanged);

	if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
	{
		MeshComp->OnComponentHit.AddDynamic(this, &ACAsteroidBase::OnAsteroidHit);
	}
}

void ACAsteroidBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bTickPhysicsHomingForce)
	{
		AddForceInPlayerDirection(GetPlayerRef());
	}
}

void ACAsteroidBase::OnHealthChanged_Implementation(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp,
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

void ACAsteroidBase::OnAsteroidHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor,
												  UPrimitiveComponent* OtherComp, FVector NormalImpulse,
												  const FHitResult& Hit)
{
}

bool ACAsteroidBase::IsAlive()
{
	return AttributeComp->IsAlive();
}

UStaticMeshComponent* ACAsteroidBase::GetStaticMeshComponent_Implementation()
{
	return nullptr;
}

FVector ACAsteroidBase::GetPlayerDirection(AActor* Player) const
{
	if (!Player)
	{
		return FVector(0);
	}
	return (Player->GetActorLocation() - GetActorLocation());
}

void ACAsteroidBase::AddForceInPlayerDirection(AActor* Player)
{
	if (!Player)
	{
		return;
	}
	if (UStaticMeshComponent* StaticMesh = GetStaticMeshComponent())
	{
		StaticMesh->AddForce(GetPlayerDirection(Player), NAME_None, true);
	}
}

AActor* ACAsteroidBase::GetPlayerRef_Implementation() const
{
	return nullptr;
}

void ACAsteroidBase::SpawnSmallerAsteroids_Implementation()
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
		if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(AsteroidClass, SpawnTM, SpawnParams))
		{
			if (UCAsteroidAttributeComponent* AsteroidAttributeComp = Cast<UCAsteroidAttributeComponent>(
					NewActor->GetComponentByClass(UCAsteroidAttributeComponent::StaticClass())))
			{
				// Pass on own attributes to children
				AsteroidAttributeComp->SetNumberOfAsteroidsToSpawn(AttributeComp->GetNumberOfAsteroidsToSpawn());
				AsteroidAttributeComp->ModifyAttributes(AttributeComp->GetAttributeModifier());
			}
		}
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

void ACAsteroidBase::SpawnItem_Implementation() {}

void ACAsteroidBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		if (AttributeComp->GetSize() != EAsteroidSize::Base)
		{
			SpawnSmallerAsteroids();
		}
		if (AttributeComp->TrySpawnItem())
		{
			SpawnItem();
		}
		Destroy();
	}
}