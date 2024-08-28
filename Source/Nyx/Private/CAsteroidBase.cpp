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
	AttributeComp->OnHealthChanged.AddDynamic(this, &ACAsteroidBase::NativeHealthChangedHandler);
}

void ACAsteroidBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (bTickPhysicsHomingForce && AttributeComp->GetState() != EAsteroidState::Stunned)
	{
		AddForceInPlayerDirection(GetPlayerRef());
	}
}

void ACAsteroidBase::NativeHealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp,
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

bool ACAsteroidBase::IsAlive()
{
	return AttributeComp->IsAlive();
}

UStaticMeshComponent* ACAsteroidBase::GetStaticMeshComponent_Implementation() const
{
	return nullptr;
}

void ACAsteroidBase::OnStunTimerComplete()
{
	ICStunInterface::Execute_Recover(this);
}

FVector ACAsteroidBase::GetPlayerDirection(AActor* Player) const
{
	if (!Player)
	{
		return FVector(0);
	}
	return (Player->GetActorLocation() - GetActorLocation());
}

void ACAsteroidBase::Stun_Implementation(float StunTime)
{
	RecoverToState = AttributeComp->GetState();
	AttributeComp->SetState(EAsteroidState::Stunned);
	EnablePhysicsAndGravity();
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ACAsteroidBase::OnStunTimerComplete, StunTime);
}

void ACAsteroidBase::Recover_Implementation()
{
	switch (RecoverToState)
	{
		case EAsteroidState::Active:
			DisablePhysicsAndGravity();
			break;
		case EAsteroidState::Aggressive:
			break;
		default:
			EnablePhysicsAndGravity();
	}
	AttributeComp->SetState(RecoverToState);
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

void ACAsteroidBase::EnablePhysicsAndGravity()
{
	if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
	{
		MeshComp->SetEnableGravity(true);
		MeshComp->SetSimulatePhysics(true);
	}
}
void ACAsteroidBase::DisablePhysicsAndGravity()
{
	if (UStaticMeshComponent* MeshComp = GetStaticMeshComponent())
	{
		MeshComp->SetEnableGravity(false);
		MeshComp->SetSimulatePhysics(false);
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