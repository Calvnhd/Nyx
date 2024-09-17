// Copyright (C) 2024 - Calvin Davidson

#include "CTargetingComponent.h"
#include "CCommonDefines.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCTargetingComponent::UCTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	FindTargetTraceRadius = 1000.0f;
}

FTransform UCTargetingComponent::GetCrosshairTargetTM(UCameraComponent* Camera, FVector MuzzleLocation) const
{
	return FTransform(UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, GetCrosshairTargetLocation(Camera, MuzzleLocation)), MuzzleLocation);
}

FTransform UCTargetingComponent::GetLockedTargetTM(FVector LockedTargetLocation, FVector MuzzleLocation) const
{
	return FTransform(UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, LockedTargetLocation), MuzzleLocation);
}

FVector UCTargetingComponent::GetCrosshairTargetLocation(UCameraComponent* Camera, FVector MuzzleLocation) const
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

AActor* UCTargetingComponent::FindTargetActor(UCameraComponent* Camera, FVector MuzzleLocation) const
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