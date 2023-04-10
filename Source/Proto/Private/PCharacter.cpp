// Copyright (C) 2023 - Calvin Davidson

#include "PCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PAttributeComponentPlayer.h"
#include "PInteractComponent.h"

// Sets default values
APCharacter::APCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Is this still going to be relevant to your camera needs?
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	InteractComp = CreateDefaultSubobject<UPInteractComponent>("InteractComp");
	AttributeComp = CreateDefaultSubobject<UPAttributeComponentPlayer>("AttributeComp");

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void APCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Should these be in AttributeComp maybe?
	// Or maybe an AttackComp?
	// Maybe both?
	DashDistance = 10.0f;
	AttackPrimaryWindUp = 0.2f;
	AttackDashWindUp = 1.0f;
	IsShielding = false;
	DamageReduction = 1.0f;
	SpeedReduction = 1.0f;

	// Delegate bindings will go here
	//AttributeComp->OnHealthChangedTrigger.AddDynamic(this, &APCharacter::OnHealthChangedResponse);
}

// Called every frame
void APCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &APCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APCharacter::AddControllerPitchInput);

	// Abilities
	PlayerInputComponent->BindAction("AttackPrimary", IE_Pressed, this, &APCharacter::AttackPrimary);
	// PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &APCharacter::AttackSpecial);
	// PlayerInputComponent->BindAction("AttackDash", IE_Pressed, this, &APCharacter::AttackDash);
	// PlayerInputComponent->BindAction("ShieldUp", IE_Pressed, this, &APCharacter::ShieldUp);
	// PlayerInputComponent->BindAction("ShieldDown", IE_Released, this, &APCharacter::ShieldDown);
	//
	// PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APCharacter::Interact);
}

void APCharacter::MoveForward(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	AddMovementInput(ControlRotation.Vector(), Value);
}
void APCharacter::MoveRight(float Value)
{
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

//void APCharacter::OnHealthChangedResponse(float Delta)
//{
//	// todo
//}

void APCharacter::AttackPrimary()
{
	// todo: Kick off some kind of attack animation, with an event/notifier to invoke the spawn
	// Set timer to spawn the projectile after some time
	GetWorldTimerManager().SetTimer(TimerHandle_AttackPrimary, this, &APCharacter::AttackPrimary_TimeElapsed,
									AttackPrimaryWindUp);
}

void APCharacter::AttackPrimary_TimeElapsed()
{
	SpawnProjectile(ProjectileClassPrimary);
}

void APCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClassToSpawn)
{
	// todo: Error checking to make sure ClassToSpawn is set in BP
	// todo: pass in an FActorSpawnParams as a third argument
	GetWorld()->SpawnActor<AActor>(ProjectileClassToSpawn, GetProjectileTM());
}

void APCharacter::AttackSpecial()
{
	GetWorldTimerManager().SetTimer(TimerHandle_AttackSpecial, this, &APCharacter::AttackSpecial_TimeElapsed,
									AttackSpecialWindUp);
}

void APCharacter::AttackSpecial_TimeElapsed()
{
	SpawnProjectile(ProjectileClassSpecial);
	// todo: Recovery
}

void APCharacter::AttackDash()
{
	// todo: Add some kind of charge animation
	GetWorldTimerManager().SetTimer(TimerHandle_AttackDash, this, &APCharacter::AttackDash_TimeElapsed,
									AttackDashWindUp);
}

void APCharacter::AttackDash_TimeElapsed()
{
	// todo: Add attack and damage detail
	// I guess...
	// What's my location right now? stick that in an FTransform
	// Where do I want to be?  Well, a few metres ahead in the direction I'm looking, I'd say
	// Set my location to whatever that is

	// FTransform PlayerTM = this->GetTransform();

	// I... I'm not sure.
	// Refer to TLUE.

	GetWorldTimerManager().SetTimer(TimerHandle_AttackDashCooldown, this, &APCharacter::AttackDashCooldown_TimeElapsed,
									AttackDashCooldown);
}

void APCharacter::AttackDashCooldown_TimeElapsed()
{
	// todo:Recovery
}

void APCharacter::ShieldUp()
{
	// todo: should I be checking that we're toggling in the right direction? Just in case?
	IsShielding = true;
}

void APCharacter::ShieldDown()
{
	// todo: should I be checking that we're toggling in the right direction? Just in case?
	IsShielding = false;
}

FTransform APCharacter::GetProjectileTM()
{
	// Where is the projectile spawning?  And what do we want it to be looking at?
	// We're after the location and rotation for the projectile to spawn.
	// An FTransform can specify scale too, but you're fine with the default of (1,1,1)

	// Todo: Get a better rotation that makes sense for the controls. Twin stick?  Lock on?
	// Spawn the projectile the the direction the character is facing
	FRotator SpawnRotation = GetActorRotation();
	// Todo: Get a better location that make sense for the animation
	// Spawn the projectile at the character's location
	FVector SpawnLocation = GetActorLocation();

	return FTransform(SpawnRotation, SpawnLocation);
}

void APCharacter::Interact()
{
	InteractComp->Interact();
}

void APCharacter::ApplyDamage_Implementation(float DamageAmount)
{
	float HealthReduction = -DamageAmount;

	if (IsShielding)
	{
		HealthReduction = HealthReduction * DamageReduction;
	}

	AttributeComp->ApplyHealthChange(HealthReduction);
	// todo
	// AttributeComp->ApplyPostureChange(PostureReduction);
}
