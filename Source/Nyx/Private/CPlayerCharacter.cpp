// Copyright (C) 2023 - Calvin Davidson

#include "CPlayerCharacter.h"

#include "CAttributeComponent.h"
#include "CCommonDefines.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPlayerCharacter::ACPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Is this still going to be relevant to your camera needs?
	bUseControllerRotationYaw = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	SpringArmComp->SetupAttachment(RootComponent);

	// Need this still?
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp->SetupAttachment(SpringArmComp);
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
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void ACPlayerCharacter::SpawnProjectile(const TSubclassOf<AActor> ClassToSpawn)
{
	// Make sure the projectile class is assigned in BP
	if (ensureAlways(ClassToSpawn))
	{
		FActorSpawnParameters SpawnParams;
		// Make sure the Projectile knows that it was spawned by the Player
		SpawnParams.Instigator = this;
		// Make projectile always spawn at desired location, regardless of collisions
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		// Spawn projectile
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, GetCrosshairTargetTM(), SpawnParams);
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
