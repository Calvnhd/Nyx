// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"
#include "CAttributeComponent.h"
#include "CCommonDefines.h"

// Sets default values
ACEnemyBase::ACEnemyBase()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	SetRootComponent(MeshComp);

	MeshComp->SetCollisionObjectType(COLLISION_ENEMY);
}
void ACEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}
void ACEnemyBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AttributeComp->OnHealthChangedDelegate.AddDynamic(this, &ACEnemyBase::OnHealthChangedResponse);
}
void ACEnemyBase::OnHealthChangedResponse(float Delta, float NewHealth) 
{
	if (NewHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enemy destroyed"));
		Destroy();
	}
}
float ACEnemyBase::GetHealthPercent()
{
	return AttributeComp->GetHealthPercent();
}

