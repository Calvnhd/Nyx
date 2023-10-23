// Copyright (C) 2023 - Calvin Davidson

#include "CTargetDummy.h"

// Sets default values
ACTargetDummy::ACTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
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
