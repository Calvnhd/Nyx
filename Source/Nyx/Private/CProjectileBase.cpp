// Copyright (C) 2023 - Calvin Davidson

#include "CProjectileBase.h"

#include "CAttributeInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACProjectileBase::ACProjectileBase()
{
	// Create components
	ImpactVFX = CreateDefaultSubobject<UParticleSystem>("ImpactVFX");
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");

	// Attachments and hierachy
	RootComponent = SphereComp;
	EffectComp->SetupAttachment(SphereComp);

	// Collision
	// todo: set up "Projectile" collision profile
	// SphereComp->SetCollisionProfileName("Projectile");

	// Movement
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	// This is low, so the override in inherited class is obvious
	MovementComp->InitialSpeed = 250.0f;

	// todo: audio
}

// void ACProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
// 									   UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)

// {
// 	float Radius = 50.0f;
// 	float Segments = 32;
// 	FColor LineColor = Hit.bBlockingHit ? FColor::Green : FColor::Red;
// 	float Lifetime = 5.0f;
// 	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, Segments, LineColor, false, Lifetime);
// 	if (GEngine)
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 15.0f, LineColor, TEXT("Projectile Hit"));
// 	}

// 	/*
// 	Delegate for notification of blocking collision against a specific component.
// 	NormalImpulse will be filled in for physics-simulating bodies, but will be zero for swept-component blocking
// 	collisions

// 	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_FiveParams(
// 	FComponentHitSignature, UPrimitiveComponent, OnComponentHit, UPrimitiveComponent*, HitComponent, AActor*,
// 	OtherActor, UPrimitiveComponent*, OtherComp, FVector, NormalImpulse, const FHitResult&, Hit);

// 	Called when a component hits (or is hit by) something solid (e.g. via movement, using Set Location with 'sweep'
// 	enabled, or physics simulation).

// 	For collisions during physics simulation to generate hit events, 'Simulation Generates Hit Events' must be enabled
// 	for this component.

// 	When receiving a hit from another object's movement, the directions of 'Hit.Normal' and 'Hit.ImpactNormal'
// 	will be adjusted to indicate force from the other object against this object.

// 	NormalImpulse will be filled in for physics-simulating bodies, but will be zero for swept-component blocking
// 	collisions.
// 	 */

// 	// Currently we just have one projectile type so we'll keep the behaviour here
// 	// Eventually will want to subclass ProjectileBase and override this behaviour

// 	// Check there's a valid OtherActor and it's not the actor who spawned this projectile (no hitting ourselves)
// 	if (OtherActor && OtherActor != GetInstigator())
// 	{
// 		// Check if what we just hit has an AttributeComponent using casting -- Cast<ExpectedType>(ThingToCast)
// 		// GetComponentByClass iterates through actor until it finds the first instance of specified class
// 		// StaticClass() lets us easily pass around the class type

// 		// todo: check if searching for the base class like this will find subclasses too.  ChatGPT suggests using
// 		// dynamic_cast? But I'm not sure that's necessary
// 		// AND, if I'm overriding this function in the child class, I hope the child version is still going to called,
// 		// even though it's type Base?
// 		// Or maybe an interface would be better here?

// 		// Has the actor we've hit implemented an Attribute interface?
// 		// Note the U prefix here
// 		if (OtherActor->Implements<UCAttributeInterface>())
// 		{
// 			// Note the I prefix here
// 			// Add Execute_ prefix to function call
// 			// First argument is the object to call this function on, followed by the function params themselves
// 			ICAttributeInterface::Execute_ApplyDamage(OtherActor, DamageAmount);
// 		}

// 		Explode();
// 	}
// }

void ACProjectileBase::OnProjectileHitResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor,
											   UPrimitiveComponent* OtherComp, FVector NormalImpulse,
											   const FHitResult& Hit)
{
	// do something on hit
	float Radius = 50.0f;
	float Segments = 32;
	FColor LineColor = Hit.bBlockingHit ? FColor::Green : FColor::Red;
	float Lifetime = 5.0f;
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, Segments, LineColor, false, Lifetime);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, LineColor, TEXT("Projectile Hit"));
	}
}

void ACProjectileBase::OnProjectileBeginOverlapResponse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
														UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
														bool bFromSweep, const FHitResult& SweepResult)
{
	// do something on overlap
	float Radius = 60.0f;
	float Segments = 32;
	FColor LineColor = SweepResult.bBlockingHit ? FColor::Yellow : FColor::Blue;
	float Lifetime = 5.0f;
	DrawDebugSphere(GetWorld(), SweepResult.ImpactPoint, Radius, Segments, LineColor, false, Lifetime);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, LineColor, TEXT("Projectile Overlap"));
	}
}

void ACProjectileBase::Explode()
{
	// todo: add ensure(IsValid(this)) to make sure we're valid and aren't already being destroyed
	if (ensure(IsValid(this)))
	{
		if (ImpactVFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		}
		Destroy();
	}
}

void ACProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings
	SphereComp->OnComponentHit.AddDynamic(this, &ACProjectileBase::OnProjectileHitResponse);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACProjectileBase::OnProjectileBeginOverlapResponse);
}

// Called when the game starts or when spawned
void ACProjectileBase::BeginPlay()
{
	Super::BeginPlay();
}

// Don't think we need this??
// Called every frame
// void ACProjectileBase::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
