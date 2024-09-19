// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerCharacter.h"

#include "CAsteroidBase.h"
#include "CCommonDefines.h"
#include "CPickupInterface.h"
#include "CPlayerAttributeComponent.h"
#include "CSkillPointsPickup.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetTraceUtils.h"

ACPlayerCharacter::ACPlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	// Controller rotation usually IS the camera rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Camera & Targeting

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0, 0, 150.0f); // with this offset
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bCameraIsLocked = false;
	LockedTarget = nullptr;
	bLookLockOverride = false;
	LookPitchCeiling = 20.0f;
	LookPitchFloor = -30.0f;
	TargetLockVelocityModifier = 1.0f;
	CameraLockDeadzoneSize = 5.0f;
	UpdateLockedTargetCounter = 0.0f;
	TimeToUpdateLockedTarget = 1.0f;
	KeepTargetLockDistance = 1000.0f;

	// Pickups

	PickupSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphereComp"));
	PickupSphereComp->SetupAttachment(RootComponent);
	HeldPickupHeight = 150.0f;
	PickupLaunchImpulseStrength = 5000.0f;
	AttackPrimaryFireRate = 1.0f;

	// Dash

	DashStrength = 4000.0f;
	DashTime = 0.2f;
	MaxSpeed = 2000.0f;
	DashDecelerationPercent = 0.9f;
	DashDecelerationRate = 0.1f;
	EndDashSpeedModifier = 0.0f;
	PowerDashMultiplier = 5.0f;
	TempInvincibleTime = 1.0f;

	// Other Components

	PlayerAttributeComp = CreateDefaultSubobject<UCPlayerAttributeComponent>(TEXT("PlayerAttributeComp"));
}

void ACPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlayerAttributeComp->OnHealthChanged.AddDynamic(this, &ACPlayerCharacter::NativeHealthChangedHandler);
	PlayerAttributeComp->OnSkillPointsChanged.AddDynamic(this, &ACPlayerCharacter::NativeSkillPointsChangedHandler);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACPlayerCharacter::NativeCapsuleCompOverlapHandler);
	PickupSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACPlayerCharacter::NativePickupSphereOverlapHandler);
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Started, this, &ACPlayerCharacter::BeginLook);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::EndLook);
		EnhancedInputComponent->BindAction(AttackPrimaryAction, ETriggerEvent::Started, this, &ACPlayerCharacter::AttackPrimary);
		EnhancedInputComponent->BindAction(AttackPrimaryAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::AttackPrimary);
		EnhancedInputComponent->BindAction(AttackSpecialAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::AttackSpecial);
		EnhancedInputComponent->BindAction(CameraLockAction, ETriggerEvent::Started, this, &ACPlayerCharacter::SetCameraLock);
		EnhancedInputComponent->BindAction(CameraLockAction, ETriggerEvent::Completed, this, &ACPlayerCharacter::SetCameraLock);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Dash);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Shield);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
		ensureAlways(false);
	}
}

void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HeldPickup)
	{
		if (UStaticMeshComponent* PickupMesh = HeldPickup->GetMesh())
		{
			PickupMesh->SetWorldLocation(GetActorLocation() + FVector(0.0f, 0.0f, HeldPickupHeight));
		}
	}
	if (bCameraIsLocked && !bLookLockOverride)
	{
		UpdateLockedTargetCounter = UpdateLockedTargetCounter + DeltaSeconds;
		CheckLockedTarget();

		// get a new target if we don't have one
		if (!LockedTarget)
		{
			SetLockedTarget();
		}
		else if (GetDistanceTo(LockedTarget) > KeepTargetLockDistance && UpdateLockedTargetCounter > TimeToUpdateLockedTarget)
		{
			SetLockedTarget();
		}
		RotateCameraToLockedTarget();

		// looking too far upwards or downwards
		float CameraPitch = FollowCamera->GetComponentRotation().Pitch;
		if (CameraPitch >= LookPitchCeiling)
		{
			// look down
			AddControllerPitchInput(0.5);
		}
		else if (CameraPitch <= LookPitchFloor)
		{
			// look up
			AddControllerPitchInput(-0.5);
		}
	}
}

void ACPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACPlayerCharacter::BeginLook(const FInputActionValue& Value)
{
	bLookLockOverride = true;
	// LockedTarget = nullptr;
}

void ACPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw input to controller
		float YawSensitivity = 1.5f;
		AddControllerYawInput(LookAxisVector.X * YawSensitivity);

		float CameraPitch = FollowCamera->GetComponentRotation().Pitch;
		if (CameraPitch < LookPitchCeiling && CameraPitch > LookPitchFloor)
		{
			AddControllerPitchInput(LookAxisVector.Y);
		}
		// looking too far upwards
		else if (CameraPitch >= LookPitchCeiling)
		{
			// look down
			AddControllerPitchInput(0.1);
		}
		// looking too far downwards
		else if (CameraPitch <= LookPitchFloor)
		{
			// look up
			AddControllerPitchInput(-0.1);
		}
	}
}

void ACPlayerCharacter::EndLook(const FInputActionValue& Value)
{
	bLookLockOverride = false;
	if (bCameraIsLocked)
	{
		SetLockedTarget();
	}
}

void ACPlayerCharacter::RotateCameraToLockedTarget_Implementation()
{
	if (!LockedTarget)
	{
		return;
	}
	FRotator CameraRotation = FollowCamera->GetComponentRotation();

	// Yaw
	float DesiredYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedTarget->GetActorLocation()).Yaw;
	float ActualYaw = CameraRotation.Yaw;
	float YawDifference = fabs(DesiredYaw - ActualYaw);
	bool bUseInner = YawDifference < 180;
	float DistanceToRotateYaw = bUseInner ? YawDifference : (360 - YawDifference);
	if (DistanceToRotateYaw < CameraLockDeadzoneSize)
	{
		return;
	}
	float RotateSpeed = (DistanceToRotateYaw / 180) * 2;
	float DirectionYaw;
	if (DesiredYaw > ActualYaw)
	{
		// Turn right for inner, left for outer
		DirectionYaw = bUseInner ? 1.0f : -1.0f;
	}
	else
	{
		// turn left for inner, right for outer
		DirectionYaw = bUseInner ? -1.0f : 1.0f;
	}
	AddControllerYawInput(RotateSpeed * DirectionYaw);

	// Pitch
	// This keeps pitch roughly between -4 and 0 when locked on target
	// float ActualPitch = CameraRotation.Pitch;
	// float DesiredPitch = -4.0f;
	// float PitchDifference = fabs(ActualPitch - DesiredPitch);
	// if (PitchDifference > 4.0f)
	//{
	//	// positive change looks down
	//	float DirectionPitch = (ActualPitch > DesiredPitch) ? 0.5 : -0.5;
	//	AddControllerPitchInput(DirectionPitch);
	//}
}

void ACPlayerCharacter::ToggleCameraLock(const FInputActionValue& Value)
{
	bCameraIsLocked = !bCameraIsLocked;
	if (!bCameraIsLocked)
	{
		LockedTarget = nullptr;
		UpdateLockedTargetCounter = 0.0f;
	}
	else
	{
		SetLockedTarget();
	}
}

void ACPlayerCharacter::SetCameraLock(const FInputActionValue& Value)
{
	bCameraIsLocked = Value.Get<bool>();
	if (!bCameraIsLocked)
	{
		LockedTarget = nullptr;
		UpdateLockedTargetCounter = 0.0f;
	}
	else
	{
		SetLockedTarget();
	}
}

void ACPlayerCharacter::SetLockedTarget()
{
	LockedTarget = nullptr;
	if (ACAsteroidBase* TargetEnemy = Cast<ACAsteroidBase>(FindNewLockedTarget()))
	{
		LockedTarget = TargetEnemy;
		// bind to on death event?
	}
}

void ACPlayerCharacter::CheckLockedTarget()
{
	if (!LockedTarget)
	{
		return;
	}
	if (!UCAttributeComponentBase ::IsActorAlive(LockedTarget))
	{
		LockedTarget = nullptr;
	}
}

FVector ACPlayerCharacter::GetCrosshairTargetLocation() const
{
	FVector CameraLocation = FollowCamera->GetComponentLocation();
	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	FVector ViewStart = CameraLocation + (CameraRotation.Vector() * 100);
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);

	FCollisionShape EnemyTraceShape;
	EnemyTraceShape.SetSphere(50.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

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

AActor* ACPlayerCharacter::FindNewLockedTarget()
{
	if (AActor* Target = SortEnemiesHit(TraceForTargets(100, 500)))
	{
		return Target;
	}
	if (AActor* Target = SortEnemiesHit(TraceForTargets(1000, 1000)))
	{
		return Target;
	}
	return SortEnemiesHit(TraceForTargets(2000, 2000));
}

TArray<FHitResult> ACPlayerCharacter::TraceForTargets(float ViewStartDistance /* = 100.0f */, float Radius /* = 500.0f */)
{
	FVector CameraLocation = FollowCamera->GetComponentLocation();
	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	FVector ViewStart = GetActorLocation() + (CameraRotation.Vector() * ViewStartDistance);
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FCollisionObjectQueryParams EnemyQueryParams;
	EnemyQueryParams.AddObjectTypesToQuery(COLLISION_ENEMY);

	TArray<FHitResult> EnemiesHit;
	GetWorld()->SweepMultiByObjectType(EnemiesHit, ViewStart, ViewEnd, FQuat::Identity, EnemyQueryParams, FCollisionShape::MakeSphere(Radius),
									   Params);
	// DrawDebugSphereTraceMulti(GetWorld(), ViewStart, ViewEnd, Radius, EDrawDebugTrace::ForDuration, !EnemiesHit.IsEmpty(), EnemiesHit,
	//						  FLinearColor::Red, FLinearColor::Green, 5.0f);

	return EnemiesHit;
}

AActor* ACPlayerCharacter::SortEnemiesHit(TArray<FHitResult> EnemiesHit)
{
	// How to prioritize targets?
	// Some combination of size, health, and proximity
	// maybe that's up to the player
	if (!EnemiesHit.IsEmpty())
	{
		AActor* ClosestEnemy = EnemiesHit.Pop().GetActor();
		FVector EnemyToPlayer = ClosestEnemy->GetActorLocation() - GetActorLocation();
		float ShortestDistance = fabs(EnemyToPlayer.Length());

		for (auto& Enemy : EnemiesHit)
		{
			FVector NextEnemyLocation = Enemy.GetActor()->GetActorLocation();
			FVector NextEnemyToPlayer = NextEnemyLocation - GetActorLocation();
			float Distance = fabs(NextEnemyToPlayer.Length());

			if (Distance < ShortestDistance)
			{
				ClosestEnemy = Enemy.GetActor();
				ShortestDistance = Distance;
			}
		}
		return ClosestEnemy;
	}
	return nullptr;
}

FTransform ACPlayerCharacter::GetTargetTM() const
{
	const FVector SpawnLocation = GetMuzzleLocation();
	FRotator SpawnRotation;
	if (bCameraIsLocked && LockedTarget)
	{
		// @TODO
		// calculate TargetLockVelocityModifier based on some combination of target velocity and distance
		FVector TargetMovementDirection = LockedTarget->GetVelocity();
		TargetMovementDirection.Normalize();
		FVector TargetLocation = LockedTarget->GetActorLocation() + TargetMovementDirection * TargetLockVelocityModifier;
		////////////////////////

		DrawDebugSphere(GetWorld(), TargetLocation, 50, 8, FColor::Red, false, 1, 0, 1);
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);
	}
	else
	{
		SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetCrosshairTargetLocation());
	}
	// A Transformation Matrix at the muzzle, looking at the target
	return FTransform(SpawnRotation, SpawnLocation);
}

float ACPlayerCharacter::CalculateBarrelPitch() const
{
	FVector MuzzleToTargetVector;
	if (bCameraIsLocked && LockedTarget)
	{
		MuzzleToTargetVector = LockedTarget->GetActorLocation() - GetMuzzleLocation();
	}
	else
	{
		MuzzleToTargetVector = GetCrosshairTargetLocation() - GetMuzzleLocation();
	}
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(MuzzleToTargetVector);
	return UKismetMathLibrary::Clamp(SpawnRotation.Pitch + NeutralBarrelPitch, MinBarrelPitch, MaxBarrelPitch);
}

float ACPlayerCharacter::CalculateTurretRotation() const
{
	if (bCameraIsLocked && LockedTarget)
	{
		FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedTarget->GetActorLocation());
		return RotationToTarget.Yaw;
	}
	return FollowCamera->GetComponentRotation().Yaw;
}

void ACPlayerCharacter::AttackPrimary(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		AttackPrimaryBegin();
	}
	else
	{
		AttackPrimaryEnd();
	}
}

void ACPlayerCharacter::AttackPrimaryBegin()
{
	if (!GetWorldTimerManager().IsTimerActive(AttackPrimaryTimerHandle))
	{
		GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryFireOnce, AttackPrimaryFireRate, true, 0);
	}
	else
	{
		float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(AttackPrimaryTimerHandle);
		GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryResetLoop, TimeRemaining, false);
	}
}
void ACPlayerCharacter::AttackPrimaryEnd()
{
	GetWorldTimerManager().ClearTimer(AttackPrimaryTimerHandle);
}

void ACPlayerCharacter::AttackPrimaryResetLoop()
{
	GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryFireOnce, AttackPrimaryFireRate, true, 0);
}

void ACPlayerCharacter::AttackPrimaryFireOnce()
{
	if (ensureAlways(ProjectileClassPrimary) && ensureAlways(MuzzleFlashPrimary))
	{
		SpawnProjectile(ProjectileClassPrimary, MuzzleFlashPrimary);
	}
}

void ACPlayerCharacter::AttackSpecial_Implementation(const FInputActionValue& Value)
{
	if (HeldPickup)
	{
		if (UStaticMeshComponent* PickupMesh = HeldPickup->GetMesh())
		{
			PickupMesh->SetSimulatePhysics(true);
			PickupMesh->SetEnableGravity(true);
		}
		if (HeldPickup->Implements<UCBombInterface>())
		{
			ICBombInterface::Execute_Arm(HeldPickup);
		}
		FRotator RotationToTarget = UKismetMathLibrary::FindLookAtRotation(HeldPickup->GetActorLocation(), GetCrosshairTargetLocation());
		HeldPickup->GetMesh()->AddImpulse(RotationToTarget.Vector() * PickupLaunchImpulseStrength, NAME_None, true);
		HeldPickup = nullptr;
	}
	else if (!OrbitingPickups.IsEmpty())
	{
		if (ACSkillPointsPickup* Pickup = Cast<ACSkillPointsPickup>(OrbitingPickups.Pop()))
		{
			HeldPickup = Pickup;
			HeldPickup->SetCanSuction(false);
			ICPickupInterface::Execute_StopSuction(HeldPickup, this);

			if (UStaticMeshComponent* PickupMesh = HeldPickup->GetMesh())
			{
				PickupMesh->SetEnableGravity(false);
				// Resets the affect of any previous forces
				PickupMesh->SetSimulatePhysics(false);
			}
		}
	}
}

void ACPlayerCharacter::Shield_Implementation(const FInputActionValue& Value)
{
	// todo
}

void ACPlayerCharacter::Dash_Implementation(const FInputActionValue& Value)
{
	float ThisDashStrength = DashStrength;
	if (HeldPickup)
	{
		MakeTempInvincibile();
		ThisDashStrength = DashStrength * PowerDashMultiplier;
		if (HeldPickup->Implements<UCBombInterface>())
		{
			ICBombInterface::Execute_Detonate(HeldPickup);
		}
		HeldPickup = nullptr;
	}
	LaunchCharacter(GetActorForwardVector() * ThisDashStrength, false, false);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &ACPlayerCharacter::OnDashComplete, DashTime);
}

void ACPlayerCharacter::OnDashComplete_Implementation()
{
	ReduceSpeedToMax();
}

void ACPlayerCharacter::MakeTempInvincibile()
{
	SetCanBeDamaged(false);
	GetWorldTimerManager().SetTimer(TempInvincibleTimerHandle, this, &ACPlayerCharacter::ExpireTempInvincible, TempInvincibleTime);
}

void ACPlayerCharacter::ExpireTempInvincible()
{
	SetCanBeDamaged(true);
}

void ACPlayerCharacter::ReduceSpeedToMax()
{
	if (GetSpeed() > MaxSpeed + EndDashSpeedModifier)
	{
		FVector MovementDirection = GetVelocity();
		MovementDirection.Normalize();
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->Velocity = GetVelocity() * DashDecelerationPercent;
			GetWorldTimerManager().SetTimer(ReduceSpeedToMaxTimerHandle, this, &ACPlayerCharacter::ReduceSpeedToMax, DashDecelerationRate);
		}
	}
}

float ACPlayerCharacter::GetSpeed() const
{
	FVector Velocity = GetVelocity();
	FVector IgnoreZ = FVector(Velocity.X, Velocity.Y, 0);
	return IgnoreZ.Length();
}

void ACPlayerCharacter::NativeHealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta, float NewHealth)
{
	if (NewHealth <= 0)
	{
		OnDeath();
	}
}

void ACPlayerCharacter::NativeSkillPointsChangedHandler(UCAttributeComponentBase* OwningComp, float Delta, float NewPoints) {}

void ACPlayerCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass)
{
	FActorSpawnParameters SpawnParams;
	// Make sure the Projectile knows that it was spawned by the Player
	SpawnParams.Instigator = this;
	// Make projectile always spawn at desired location, regardless of collisions
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Spawn projectile
	GetWorld()->SpawnActor<AActor>(ProjectileClass, GetTargetTM(), SpawnParams);
}
void ACPlayerCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass, TObjectPtr<UParticleSystem> MuzzleEffect)
{
	UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleEffect, GetMuzzleLocation(),
											 UKismetMathLibrary::MakeRotFromX(GetCrosshairTargetLocation() - GetMuzzleLocation()));

	SpawnProjectile(ProjectileClass);
}

FVector ACPlayerCharacter::GetMuzzleLocation_Implementation() const
{
	// Overridden with actual location in BP
	// Named socket would be better for a more complex mesh
	return GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, 100);
}

void ACPlayerCharacter::CollectPickup(ACSkillPointsPickup* NewPickup)
{
	OrbitingPickups.Add(NewPickup);
}

void ACPlayerCharacter::HealSelf(float Amount /* = 1000 */)
{
	PlayerAttributeComp->ApplyHealthChange(this, Amount);
}

void ACPlayerCharacter::NativePickupSphereOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
														 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UCPickupInterface>())
	{
		if (ACSkillPointsPickup* Pickup = Cast<ACSkillPointsPickup>(OtherActor))
		{
			OrbitingPickups.Add(Pickup);
		}
		ICPickupInterface::Execute_BeginSuction(OtherActor, this);
	}
}

void ACPlayerCharacter::NativeCapsuleCompOverlapHandler(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
														int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor->Implements<UCPickupInterface>())
	{
		ICPickupInterface::Execute_ConsumePickup(OtherActor, this);
	}*/
}

void ACPlayerCharacter::OnDeath_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
}
