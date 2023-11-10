// Copyright (C) 2023 - Calvin Davidson

#include "CPlayerCharacter.h"

#include "CAttributeComponent.h"
//#include "CCommonDefines.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPlayerCharacter::ACPlayerCharacter()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Nyx-SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Nyx-CameraComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("Nyx-AttributeComp");
}

// Called when the game starts or when spawned
void ACPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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
