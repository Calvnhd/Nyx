// Copyright (C) 2024 - Calvin Davidson

#include "CPlayerCharacter.h"

#include "CCommonDefines.h"
#include "CPlayerAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0, 0, 150.0f); // with this offset
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PlayerAttributeComp = CreateDefaultSubobject<UCPlayerAttributeComponent>(TEXT("PlayerAttributeComp"));
}

void ACPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PlayerAttributeComp->OnHealthChanged.AddDynamic(this, &ACPlayerCharacter::OnHealthChangedResponse);
}

void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(AttackPrimaryAction, ETriggerEvent::Started, this,
										   &ACPlayerCharacter::AttackPrimary);
		EnhancedInputComponent->BindAction(AttackPrimaryAction, ETriggerEvent::Completed, this,
										   &ACPlayerCharacter::AttackPrimary);
		EnhancedInputComponent->BindAction(AttackSpecialAction, ETriggerEvent::Triggered, this,
										   &ACPlayerCharacter::AttackSpecial);
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

void ACPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

FVector ACPlayerCharacter::GetCameraTargetLocation() const
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

	// float Radius = 5.0f;
	// float Segments = 8;
	// float Lifetime = 5.0f;
	if (GetWorld()->SweepSingleByObjectType(EnemyHit, ViewStart, ViewEnd, FQuat::Identity, EnemyQueryParams,
											EnemyTraceShape, Params))
	{
		// DrawDebugSphere(GetWorld(), EnemyHit.ImpactPoint, Radius, Segments, FColor::Red, false, Lifetime);
		return EnemyHit.ImpactPoint;
	}
	FHitResult WorldHit;
	FCollisionObjectQueryParams WorldQueryParams;
	WorldQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	FCollisionShape WorldTraceShape;
	WorldTraceShape.SetSphere(5.0f);

	if (GetWorld()->SweepSingleByObjectType(WorldHit, ViewStart, ViewEnd, FQuat::Identity, WorldQueryParams,
											WorldTraceShape, Params))
	{
		// DrawDebugSphere(GetWorld(), WorldHit.ImpactPoint, Radius, Segments, FColor::Blue, false, Lifetime);
		return WorldHit.ImpactPoint;
	}
	// DrawDebugSphere(GetWorld(), ViewEnd, Radius, Segments, FColor::Yellow, false, Lifetime);
	return ViewEnd;
}

float ACPlayerCharacter::CalculateBarrelPitch() const
{
	FVector MuzzleToTargetVector = GetCameraTargetLocation() - GetMuzzleLocation();
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(MuzzleToTargetVector);
	return UKismetMathLibrary::Clamp(SpawnRotation.Pitch + NeutralBarrelPitch, MinBarrelPitch, MaxBarrelPitch);
}
FTransform ACPlayerCharacter::GetCrosshairTargetTM() const
{
	const FVector SpawnLocation = GetMuzzleLocation();
	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, GetCameraTargetLocation());

	// DrawDebugLine(GetWorld(), SpawnLocation, (SpawnLocation + (SpawnRotation.Vector() * 100000)), FColor::Green,
	// false, 2.0f, 0, 2.0f);

	// A Transformation Matrix at the muzzle, looking at the target
	return FTransform(SpawnRotation, SpawnLocation);
}
void ACPlayerCharacter::AttackPrimary_Implementation(const FInputActionValue& Value)
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
		GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryFireOnce,
										AttackPrimaryFireRate, true, 0);
	}
	else
	{
		float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(AttackPrimaryTimerHandle);
		GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryResetLoop,
										TimeRemaining, false);
	}
}
void ACPlayerCharacter::AttackPrimaryEnd()
{
	float TimeRemaining = GetWorldTimerManager().GetTimerRemaining(AttackPrimaryTimerHandle);
	GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::DoNothing, TimeRemaining,
									false);
}

void ACPlayerCharacter::AttackPrimaryResetLoop()
{
	GetWorldTimerManager().SetTimer(AttackPrimaryTimerHandle, this, &ACPlayerCharacter::AttackPrimaryFireOnce,
									AttackPrimaryFireRate, true, 0);
}

void ACPlayerCharacter::AttackPrimaryFireOnce_Implementation()
{
	if (ensureAlways(ProjectileClassPrimary) && ensureAlways(MuzzleFlashPrimary))
	{
		SpawnProjectile(ProjectileClassPrimary, MuzzleFlashPrimary);
	}
}

void ACPlayerCharacter::AttackSpecial_Implementation(const FInputActionValue& Value)
{
	// todo
}

void ACPlayerCharacter::Dash_Implementation(const FInputActionValue& Value)
{
	// todo
}

void ACPlayerCharacter::Shield_Implementation(const FInputActionValue& Value)
{
	// todo
}

void ACPlayerCharacter::OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp,
												float Delta, float NewHealth)
{
	if (NewHealth <= 0)
	{
		OnDeath();
	}
}
void ACPlayerCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass)
{
	FActorSpawnParameters SpawnParams;
	// Make sure the Projectile knows that it was spawned by the Player
	SpawnParams.Instigator = this;
	// Make projectile always spawn at desired location, regardless of collisions
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Spawn projectile
	GetWorld()->SpawnActor<AActor>(ProjectileClass, GetCrosshairTargetTM(), SpawnParams);
}
void ACPlayerCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass, TObjectPtr<UParticleSystem> MuzzleEffect)
{
	FActorSpawnParameters SpawnParams;
	// Make sure the Projectile knows that it was spawned by the Player
	SpawnParams.Instigator = this;
	// Make projectile always spawn at desired location, regardless of collisions
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Spawn projectile
	GetWorld()->SpawnActor<AActor>(ProjectileClass, GetCrosshairTargetTM(), SpawnParams);
	UGameplayStatics::SpawnEmitterAtLocation(
		this, MuzzleEffect, GetMuzzleLocation(),
		UKismetMathLibrary::MakeRotFromX(GetCameraTargetLocation() - GetMuzzleLocation()));
}

FVector ACPlayerCharacter::GetMuzzleLocation_Implementation() const
{
	// Ideally would want a named socket on the mesh and call something like...
	// GetMesh()->GetSocketLocation(HandSocketName);
	return GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, 100);
}

void ACPlayerCharacter::HealSelf(float Amount /* = 1000 */)
{
	PlayerAttributeComp->ApplyHealthChange(this, Amount);
}

void ACPlayerCharacter::OnDeath_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("YOU DIED"));
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	DisableInput(PlayerController);
}

void ACPlayerCharacter::DoNothing() {}