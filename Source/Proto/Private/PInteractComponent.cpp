// Copyright (C) 2023 - Calvin Davidson

#include "PInteractComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPInteractComponent::UPInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPInteractComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPInteractComponent::Interact()
{
	// LineTraceSingle will detect first object only I think?
	// Channel, object, profile
	// Using Object type because we're after any WorldDynamic
	// You can probably make your own profiles, like you have been with collisions already!

	// FHitResult Will be filled with data
	// FHitResult Hit;
	// Collision rules
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	// GetOwner() in context of component, you'll get whatever you're attached to.
	AActor* MyOwner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	// This line trace is a small single line
	// bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// Or, a variation...
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	// Sweep -- Takes a sphere and moves it from start to end
	// Multi -- Stop on first block, and collect all overlaps on the way
	// FQuat::Identity is like an empty rotation.  Quaternion is a more complicated (more info) rotator.  Quats aren't
	// exposed to BP.
	bool bBlockingHit =
		GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	// Expensive loop?
	for (FHitResult Hit : Hits)
	{
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);

		// Hit now has heaps of info to use
		// We want to call the actor that we've hit.

		// if (AActor* HitActor = Hit.GetActor())
		//{
		//	// Has the actor we've hit implemented an Interact interface?
		//	if (HitActor->Implements<UPGameplayInterface>())
		//	{
		//		// now we can call the function on it
		//		// Syntax adds Execute_ prefix
		//		// First argument is now the Actor we want to call the function on, with second arg being instigator
		//		// (i.e. whatever this component is attached to) We specified instigator to be a pawn, so we have to
		//		// cast it.  This casting is safe, unlike raw C++
		//		APawn* MyPawn = Cast<APawn>(MyOwner);
		//		IPGameplayInterface::Execute_Interact(HitActor, MyPawn);
		//		//  break after we find the first AActor so we don't interact with multiple objects
		//		break;
		//	}
		// }
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f, 0, 2.0f);
}
