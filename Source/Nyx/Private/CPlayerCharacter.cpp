// Copyright (C) 2023 - Calvin Davidson

#include "CPlayerCharacter.h"

#include "CAttributeComponent.h"
#include "CCommonDefines.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 500.0f; // The camera follows at this distance behind the character
	SpringArmComp->SocketOffset = FVector(0, 0, 150.0f); // with this offset
	SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCapsuleComponent()->InitCapsuleSize(110.0f, 110.0f);

	// Don't rotate when the controller rotates.
	// Let that just affect the camera.
	// Controller rotation usually IS the camera rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, MovementRotationRate, 0.0f); // ...at this rotation rate

	// This is taken from the third person example, left here as a reminder
	// Find which variables you're using often and give them reasonable defaults
	// GetCharacterMovement()->JumpZVelocity = 700.f;
	// GetCharacterMovement()->AirControl = 0.35f;
	// GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	// GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}
// Called when the game starts or when spawned
void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComp->OnPlayerHealthChangedDelegate.AddDynamic(this, &ACPlayerCharacter::OnHealthChangedResponse);
}
// Called every frame
void ACPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void ACPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Set up gameplay key bindings */

	// Actions
	PlayerInputComponent->BindAction("AttackPrimary", IE_Pressed, this, &ACPlayerCharacter::SpawnProjectile);

	// Movement
	PlayerInputComponent->BindAxis("MoveForwardBackward", this, &ACPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRightLeft", this, &ACPlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("TurnMouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnGamepad", this, &ACPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookMouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookGamepad", this, &ACPlayerCharacter::LookUpAtRate);
}
void ACPlayerCharacter::SpawnProjectile()
{
	// Make sure the projectile class is assigned in BP
	if (ensureAlways(ProjectileClass))
	{
		FActorSpawnParameters SpawnParams;
		// Make sure the Projectile knows that it was spawned by the Player
		SpawnParams.Instigator = this;
		// Make projectile always spawn at desired location, regardless of collisions
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Spawn projectile
		GetWorld()->SpawnActor<AActor>(ProjectileClass, GetCrosshairTargetTM(), SpawnParams);
	}
}

void ACPlayerCharacter::OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float Delta,
												float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
									 FString::Printf(TEXT("Health changed by %f, health now %f"), NewHealth));
	if (NewHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("YOU DIED"));
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
		// todo add death effect
	}
}

FTransform ACPlayerCharacter::GetCrosshairTargetTM()
{
	// You want to know where you're looking from
	FVector CameraLocation = CameraComp->GetComponentLocation();
	// What direction you're looking
	FRotator CameraRotation = CameraComp->GetComponentRotation();
	// What's your maximum view distance?
	FVector ViewEnd = CameraLocation + (CameraRotation.Vector() * 10000);

	// What do you see?
	FHitResult ViewHit;
	// This is a list of all the object types we're looking for
	FCollisionObjectQueryParams ObjectQueryParams;
	if (!bIsAutoAimActive)
	{
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	}
	ObjectQueryParams.AddObjectTypesToQuery(COLLISION_ENEMY);

	// This is the shape of the trace.  A sphere is more lenient than a line.
	FCollisionShape TraceShape;
	float TraceRadius = bIsAutoAimActive ? AutoAimSweepRadius : 20.0f;
	TraceShape.SetSphere(TraceRadius);

	// Ignore player
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Create trace
	bool bBlockingHit = GetWorld()->SweepSingleByObjectType(ViewHit, CameraLocation, ViewEnd, FQuat::Identity,
															ObjectQueryParams, TraceShape, Params);
	// that will give you a target location
	FVector Target = bBlockingHit ? ViewHit.ImpactPoint : ViewEnd;

	if (bBlockingHit)
	{
		float Radius = 50.0f;
		float Segments = 32;
		float Lifetime = 5.0f;
		DrawDebugSphere(GetWorld(), ViewHit.ImpactPoint, Radius, Segments, FColor::MakeRandomColor(), false, Lifetime);
	}
	// then you want a spawn location for the projectile
	// todo -- make a socket on the mesh and give it a name
	FVector SpawnLocation = GetMuzzleLocation();
	// and a rotation for that spawn location, looking in the direction of the target
	// Target - SpawnLocation calculates the vector from SpawnLocation to Target. This vector points from SpawnLocation
	// towards Target.
	FRotator SpawnRotation = UKismetMathLibrary::MakeRotFromX(Target - SpawnLocation);

	// Debug info
	FColor SightColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), SpawnLocation, (SpawnLocation + (SpawnRotation.Vector() * 100000)), SightColor, false,
				  2.0f, 0, 2.0f);

	// A Transformation Matrix above the ship, looking at the target
	return FTransform(SpawnRotation, SpawnLocation);
}

FVector ACPlayerCharacter::GetMuzzleLocation()
{
	// Quick n dirty for now
	// Eventually would want to have a socket on the mesh and call something like
	// GetMesh()->GetSocketLocation(HandSocketName);
	return GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, MuzzleHeightOffset);
}

void ACPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ACPlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACPlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
