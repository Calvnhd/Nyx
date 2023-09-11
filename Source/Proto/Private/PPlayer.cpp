// Copyright (C) 2023 - Calvin Davidson

#include "PPlayer.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PAttributeComponent.h"

// Sets default values
APPlayer::APPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Is this still going to be relevant to your camera needs?
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	AttributeComp = CreateDefaultSubobject<UPAttributeComponent>("AttributeComp");

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp->SetupAttachment(SpringArmComp);
}
// Called when the game starts or when spawned
void APPlayer::BeginPlay()
{
	Super::BeginPlay();
}
void APPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings will go here e.g.
	// AttributeComp->OnHealthChangedTrigger.AddDynamic(this, &APPlayer::OnHealthChangedResponse);
}
// Called every frame
void APPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void APPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAction("SetThrustToMax", IE_Pressed, this, &APPlayer::SetThrustToMax);
	PlayerInputComponent->BindAction("SetThrustToMin", IE_Pressed, this, &APPlayer::SetThrustToMin);
}
// void APPlayer::IncreaseThrust() {}
// void APPlayer::DecreaseThrust() {}
void APPlayer::SetThrustToMax()
{
	AttributeComp->SetThrustToMax();
}
void APPlayer::SetThrustToMin()
{
	AttributeComp->SetThrustToMin();
}
// void APPlayer::RotateRight() {}
// void APPlayer::RotateLeft() {}
// void APPlayer::PitchUp() {}
// void APPlayer::PitchDown() {}
