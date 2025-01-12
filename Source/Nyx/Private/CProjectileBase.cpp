// Copyright (C) 2024 - Calvin Davidson

#include "CProjectileBase.h"

#include "AI/CAsteroidAttributeComponent.h"
#include "AI/CEnemyAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACProjectileBase::ACProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	ImpactVFX = CreateDefaultSubobject<UParticleSystem>("ImpactVFX");
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");
	ForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");

	// Attachments and hierarchy
	RootComponent = SphereComp;
	EffectComp->SetupAttachment(SphereComp);
	ForceComp->SetupAttachment(SphereComp);

	// Collision
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->SetSimulatePhysics(true);

	// Movement
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 250.0f;

	// Other properties
	DamageAmount = 10;
	// Lifetime
	MaximumLifetime = 1.0f;
}

void ACProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentHit.AddDynamic(this, &ACProjectileBase::NativeProjectileHitHandler);
}

void ACProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Don't hit yourself
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	// Don't live too long
	SetLifeSpan(MaximumLifetime);
}

void ACProjectileBase::NativeProjectileHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor,
												  UPrimitiveComponent* OtherComp, FVector NormalImpulse,
												  const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		//if (UCAsteroidAttributeComponent* AttributeComp =
		//		Cast<UCAsteroidAttributeComponent>(UCAttributeComponentBase::GetAttributes(OtherActor)))
		//{
		//	AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);
		//}
		if (UCAttributeComponentBase* AttributeComp = UCAttributeComponentBase::GetAttributes(OtherActor))
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);
		}
		Explode();
	}
}

void ACProjectileBase::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		if (ImpactVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		}
		Destroy();
	}
}
