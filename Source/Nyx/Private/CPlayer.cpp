// Copyright (C) 2023 - Calvin Davidson

#include "CPlayer.h"

#include "CAttributeComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACPlayer::ACPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Is this still going to be relevant to your camera needs?
	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	
	MeshComp->SetupAttachment(RootComponent);

	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp->SetupAttachment(SpringArmComp);
}
// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
}
void ACPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings will go here e.g.
	// AttributeComp->OnHealthChangedTrigger.AddDynamic(this, &ACPlayer::OnHealthChangedResponse);
}
// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAction("SetThrustToMax", IE_Pressed, this, &ACPlayer::SetThrustToMax);
	PlayerInputComponent->BindAction("SetThrustToMin", IE_Pressed, this, &ACPlayer::SetThrustToMin);
}
// void ACPlayer::IncreaseThrust() {}
// void ACPlayer::DecreaseThrust() {}
void ACPlayer::SetThrustToMax()
{
	AttributeComp->SetThrustToMax();
}
void ACPlayer::SetThrustToMin()
{
	AttributeComp->SetThrustToMin();
}
// void ACPlayer::RotateRight() {}
// void ACPlayer::RotateLeft() {}
// void ACPlayer::PitchUp() {}
// void ACPlayer::PitchDown() {}
