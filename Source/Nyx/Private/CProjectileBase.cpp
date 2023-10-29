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
	PrimaryActorTick.bCanEverTick = false;

	// Create components
	ImpactVFX = CreateDefaultSubobject<UParticleSystem>("ImpactVFX");
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComp");

	// Attachments and hierarchy
	RootComponent = SphereComp;
	EffectComp->SetupAttachment(SphereComp);

	// Collision
	SphereComp->SetCollisionProfileName("Projectile");
	

	// Movement
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->InitialSpeed = 250.0f;

	// Other properties
	DamageAmount = 10;
}

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

	// Check there's a valid OtherActor and it's not the actor who spawned this projectile (no hitting ourselves)
	if (OtherActor && OtherActor != GetInstigator())
	{
		// Check if what we just hit has an AttributeComponent using casting -- Cast<ExpectedType>(ThingToCast)
		// GetComponentByClass iterates through actor until it finds the first instance of specified class
		// StaticClass() lets us easily pass around the class type

		// Has the actor we've hit implemented an Attribute interface?
		// Note the U prefix here
		// if (OtherActor->Implements<UCAttributeInterface>())
		//{
		// Note the I prefix here
		// Add Execute_ prefix to function call
		// First argument is the object to call this function on, followed by the function params themselves
		//	ICAttributeInterface::Execute_ApplyDamage(OtherActor, DamageAmount);
		//}

		Explode();
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

	if (OtherActor && OtherActor != GetInstigator())
	{
		Explode();
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Projectile explode"));
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

	// Don't hit yourself
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}