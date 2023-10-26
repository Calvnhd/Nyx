// Copyright (C) 2023 - Calvin Davidson

#include "CTargetDummy.h"
#include "CEnemyAttributeComponent.h"


// Sets default values
ACTargetDummy::ACTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	AttributeComp = CreateDefaultSubobject<UCEnemyAttributeComponent>("AttributeComp");

	SetRootComponent(MeshComp);
}
// Called when the game starts or when spawned
void ACTargetDummy::BeginPlay()
{
	Super::BeginPlay();
}
void ACTargetDummy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Delegate bindings will go here
}

void ACTargetDummy::ApplyDamage_Implementation(APawn* AttackingPawn, float DamageAmount)
{
	AttributeComp->ApplyDamage(DamageAmount);
}
