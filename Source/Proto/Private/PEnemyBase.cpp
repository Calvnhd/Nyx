// Copyright (C) 2023 - Calvin Davidson

#include "PEnemyBase.h"

#include "PAttributeComponentEnemy.h"

// Sets default values
APEnemyBase::APEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
	// it.
	PrimaryActorTick.bCanEverTick = true;
	AttributeComp = CreateDefaultSubobject<UPAttributeComponentEnemy>("AttributeComp");
}

// Called when the game starts or when spawned
void APEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void APEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void APEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}