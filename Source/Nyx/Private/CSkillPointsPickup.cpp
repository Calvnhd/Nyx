// Copyright (C) 2024 - Calvin Davidson

#include "CSkillPointsPickup.h"

#include "AI/CAsteroidBase.h"
#include "CPlayerAttributeComponent.h"
#include "Components/SphereComponent.h"

ACSkillPointsPickup::ACSkillPointsPickup()
{
	BlastSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BlastSphere"));
	BlastSphere->SetupAttachment(RootComponent);

	PointsValue = 10.0f;
	bCanSuction = true;
	OrbitForceMultiplier = 2.5f;
	OrbitRadiusThreshold = 250.0f;
	RepelForceMultiplier = 1.0f;
	bIsArmed = false;
	BlastDamageAmount = 100.0f;
	CollisionDamageAmount = 10.0f;
	StunTimeAmount = 10.0f;
}
void ACSkillPointsPickup::SetCanSuction(bool bNewCanSuction)
{
	bCanSuction = bNewCanSuction;
}
void ACSkillPointsPickup::Tick(float DeltaSeconds)
{
	if (bIsSuctionActive && PlayerRef)
	{
		FVector PickupToPlayer = (PlayerRef->GetActorLocation() - GetActorLocation());
		float Distance = PickupToPlayer.Length();
		PickupToPlayer.Normalize();
		FVector PerpendicularVector = FRotator(0.0f, 90.0f, 0.0f).RotateVector(PickupToPlayer);

		MeshComp->AddForce(PickupToPlayer * SuctionForceMultiplier * Distance, NAME_None, true);
		MeshComp->AddForce(PerpendicularVector * OrbitForceMultiplier * (1 / Distance) * 100000, NAME_None, true);
		if (Distance < OrbitRadiusThreshold)
		{
			MeshComp->AddForce(-1 * PickupToPlayer * SuctionForceMultiplier * SuctionForceMultiplier *
								   RepelForceMultiplier * (1 / Distance) * 100000,
							   NAME_None, true);
		}
	}
}

void ACSkillPointsPickup::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &ACSkillPointsPickup::NativeComponentHitHandler);
}

void ACSkillPointsPickup::ConsumePickup_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		if (UCPlayerAttributeComponent* Attributes =
				Cast<UCPlayerAttributeComponent>(UCAttributeComponentBase::GetAttributes(InstigatorPawn)))
		{
			Attributes->AddSkillPoints(PointsValue);
		}
	}
	Super::ConsumePickup_Implementation(InstigatorPawn);
}
void ACSkillPointsPickup::BeginSuction_Implementation(APawn* InstigatorPawn)
{
	if (bCanSuction)
	{
		Super::BeginSuction_Implementation(InstigatorPawn);
	}
}
void ACSkillPointsPickup::StopSuction_Implementation(APawn* InstigatorPawn)
{
	Super::StopSuction_Implementation(InstigatorPawn);
}
void ACSkillPointsPickup::Arm_Implementation()
{
	bIsArmed = true;
}
void ACSkillPointsPickup::Detonate_Implementation()
{
	Destroy();
}

void ACSkillPointsPickup::NativeComponentHitHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor,
													UPrimitiveComponent* OtherComp, FVector NormalImpulse,
													const FHitResult& Hit)
{
	if (bIsArmed)
	{
		Execute_Detonate(this);
	}
}

void ACSkillPointsPickup::BlastDamageAndStun()
{
	if (!BlastSphere)
	{
		return;
	}
	TArray<AActor*> OverlappingActors;
	BlastSphere->GetOverlappingActors(OverlappingActors);
	if (OverlappingActors.IsEmpty())
	{
		return;
	}
	for (auto Actor : OverlappingActors)
	{
		if (UCAsteroidAttributeComponent* Attributes =
				Cast<UCAsteroidAttributeComponent>(UCAttributeComponentBase::GetAttributes(Actor)))
		{
			Attributes->ApplyHealthChange(this, -BlastDamageAmount);
		}
		if (Actor->Implements<UCStunInterface>())
		{
			ICStunInterface::Execute_Stun(Actor, StunTimeAmount);
		}
	}
}