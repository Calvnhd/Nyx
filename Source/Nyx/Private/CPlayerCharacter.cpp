// Copyright (C) 2023 - Calvin Davidson

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
	GetCapsuleComponent()->InitCapsuleSize(110.0f, 110.0f);

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
	PlayerAttributeComp->OnHealthChangedDelegate.AddDynamic(this, &ACPlayerCharacter::OnHealthChangedResponse);
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
		EnhancedInputComponent->BindAction(AttackPrimaryAction, ETriggerEvent::Triggered, this,
										   &ACPlayerCharacter::AttackPrimary);
		EnhancedInputComponent->BindAction(AttackSpecialAction, ETriggerEvent::Triggered, this,
										   &ACPlayerCharacter::AttackSpecial);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Dash);
		EnhancedInputComponent->BindAction(ShieldAction, ETriggerEvent::Triggered, this, &ACPlayerCharacter::Shield);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
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

FVector ACPlayerCharacter::GetViewTargetLocation() const
{
	// You want to know where you're looking from
	FVector CameraLocation = FollowCamera->GetComponentLocation();
	// What direction you're looking
	FRotator CameraRotation = FollowCamera->GetComponentRotation();
	// What's your maximum view distance?
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);
	// What do you see?
	FHitResult ViewHit;
	// This is a list of all the object types we're looking for
	FCollisionObjectQueryParams ObjectQueryParams;
	// if (!bIsAutoAimActive)
	//{
	//	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	//	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	//	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	// }
	ObjectQueryParams.AddObjectTypesToQuery(COLLISION_ENEMY);

	// This is the shape of the trace.  A sphere is more lenient than a line.
	FCollisionShape TraceShape;
	// float TraceRadius = bIsAutoAimActive ? AutoAimSweepRadius : 20.0f;
	float TraceRadius = 20.0f;
	TraceShape.SetSphere(TraceRadius);

	// Ignore player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Create trace
	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(ViewHit, CameraLocation, ViewEnd, FQuat::Identity,
															ObjectQueryParams, TraceShape, Params);

	// if (bBlockingHit)
	//{
	//	float Radius = 50.0f;
	//	float Segments = 32;
	//	float Lifetime = 5.0f;
	//	DrawDebugSphere(GetWorld(), ViewHit.ImpactPoint, Radius, Segments, FColor::MakeRandomColor(), false, Lifetime);
	// }

	// that will give you a target location
	return bBlockingHit ? ViewHit.ImpactPoint : ViewEnd;
}

float ACPlayerCharacter::CalculateBarrelPitch() const
{
	FVector MuzzleToTargetVector = GetViewTargetLocation() - GetMuzzleLocation();
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(MuzzleToTargetVector);
	return UKismetMathLibrary::Clamp(SpawnRotation.Pitch + NeutralBarrelPitch, MinBarrelPitch, MaxBarrelPitch);
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

void ACPlayerCharacter::AttackPrimaryFireOnce()
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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("YOU DIED"));
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
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
		UKismetMathLibrary::MakeRotFromX(GetViewTargetLocation() - GetMuzzleLocation()));
}
FTransform ACPlayerCharacter::GetCrosshairTargetTM()
{
	FVector Target = GetViewTargetLocation();

	// then you want a spawn location for the projectile...
	FVector SpawnLocation = GetMuzzleLocation();
	// ...and a rotation for that spawn location, looking in the direction of the target
	// Target - SpawnLocation calculates the vector from SpawnLocation to Target.
	// This vector points from SpawnLocation towards Target.
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(Target - SpawnLocation);

	// Debug info
	// FColor SightColor = bBlockingHit ? FColor::Green : FColor::Red;
	// DrawDebugLine(GetWorld(), SpawnLocation, (SpawnLocation + (SpawnRotation.Vector() * 100000)), SightColor,
	// false,
	//              2.0f, 0, 2.0f);

	// A Transformation Matrix above the ship, looking at the target
	return FTransform(SpawnRotation, SpawnLocation);
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
void ACPlayerCharacter::DoNothing() {}