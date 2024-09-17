// Copyright (C) 2024 - Calvin Davidson

#include "CTargetingComponent.h"
#include "CCommonDefines.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	TargetLockedActor = nullptr;
	FindTargetTraceRadius = 1000.0f;

	Camera = nullptr;
	Muzzle = nullptr;
}

void UCTargetingComponent::InitializeReferences(UCameraComponent* InCamera, AActor* InMuzzle)
{
	ensureAlways(InCamera);
	ensureAlways(InMuzzle);
	Camera = InCamera;
	Muzzle = InMuzzle;
}

FTransform UCTargetingComponent::GetCrosshairTargetTM() const
{
	const FVector SpawnLocation = Muzzle->GetActorLocation();
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetCameraTargetLocation());

	// A Transformation Matrix at the muzzle, looking at the target
	return FTransform(SpawnRotation, SpawnLocation);
}

FTransform UCTargetingComponent::GetLockedTargetTM() const
{
	if (TargetLockedActor)
	{
		const FVector SpawnLocation = Muzzle->GetActorLocation();
		const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLockedActor->GetActorLocation());

		// A Transformation Matrix at the muzzle, looking at the target
		return FTransform(SpawnRotation, SpawnLocation);
	}
	ensure(false);
	return FTransform();
}

FVector UCTargetingComponent::GetCameraTargetLocation() const
{
	FVector CameraLocation = Camera->GetComponentLocation();
	FRotator CameraRotation = Camera->GetComponentRotation();
	FVector ViewStart = CameraLocation + (CameraRotation.Vector() * 100);
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);

	FCollisionShape EnemyTraceShape;
	EnemyTraceShape.SetSphere(50.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FHitResult EnemyHit;
	FCollisionObjectQueryParams EnemyQueryParams;
	EnemyQueryParams.AddObjectTypesToQuery(COLLISION_ENEMY);

	if (GetWorld()->SweepSingleByObjectType(EnemyHit, ViewStart, ViewEnd, FQuat::Identity, EnemyQueryParams, EnemyTraceShape, Params))
	{
		return EnemyHit.ImpactPoint;
	}
	FHitResult WorldHit;
	FCollisionObjectQueryParams WorldQueryParams;
	WorldQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	FCollisionShape WorldTraceShape;
	WorldTraceShape.SetSphere(5.0f);

	if (GetWorld()->SweepSingleByObjectType(WorldHit, ViewStart, ViewEnd, FQuat::Identity, WorldQueryParams, WorldTraceShape, Params))
	{
		return WorldHit.ImpactPoint;
	}
	return ViewEnd;
}

AActor* UCTargetingComponent::GetCameraTargetActor() const
{
	FVector CameraLocation = Camera->GetComponentLocation();
	FRotator CameraRotation = Camera->GetComponentRotation();
	FVector ViewStart = CameraLocation + (CameraRotation.Vector() * 100);
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);

	FCollisionShape EnemyTraceShape;
	EnemyTraceShape.SetSphere(FindTargetTraceRadius);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FHitResult EnemyHit;
	FCollisionObjectQueryParams EnemyQueryParams;
	EnemyQueryParams.AddObjectTypesToQuery(COLLISION_ENEMY);

	if (GetWorld()->SweepSingleByObjectType(EnemyHit, ViewStart, ViewEnd, FQuat::Identity, EnemyQueryParams, EnemyTraceShape, Params))
	{
		// SweepSingle will return the first enemy hit
		return EnemyHit.GetActor();
	}
	return nullptr;
}

// void UCTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
//										 FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
// }

void UCTargetingComponent::ClearTargetLockedActor()
{
	TargetLockedActor = nullptr;
}

bool UCTargetingComponent::HasTargetLockedActor() const
{
	if (TargetLockedActor)
	{
		return true;
	}
	return false;
}

bool UCTargetingComponent::FindNewTargetActor()
{
	return false;
}
