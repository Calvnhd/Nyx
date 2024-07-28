// Copyright (C) 2023 - Calvin Davidson

#include "CProjectileBase.h"

#include "CEnemyAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
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
}

void ACProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
											   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
											   const FHitResult& Hit)
{
	// do something on hit
	/*float Radius = 50.0f;
	float Segments = 32;
	FColor LineColor = Hit.bBlockingHit ? FColor::Green : FColor::Red;
	float Lifetime = 5.0f;
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, Segments, LineColor, false, Lifetime);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, LineColor, TEXT("Projectile Hit"));
	}*/

	// Check there's a valid OtherActor and it's not the actor who spawned this projectile (no hitting ourselves)
	if (OtherActor && OtherActor != GetInstigator())
	{
		// Check if what we just hit has an EnemyAttributeComponent using casting -- Cast<ExpectedType>(ThingToCast)
		//
		// GetComponentByClass iterates through actor until it finds the FIRST instance of specified class
		// StaticClass() lets us easily pass around the class type.  Use this to see if the actor has a
		// CEnemyAttributeComponent, and then call the desired function on it.
		if (UCEnemyAttributeComponent* AttributeComp = Cast<UCEnemyAttributeComponent>(
				OtherActor->GetComponentByClass(UCEnemyAttributeComponent::StaticClass())))
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
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Projectile explode"));
		Destroy();
	}
}

void ACProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings
	SphereComp->OnComponentHit.AddDynamic(this, &ACProjectileBase::OnProjectileHit);
}

// Called when the game starts or when spawned
void ACProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Don't hit yourself
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}