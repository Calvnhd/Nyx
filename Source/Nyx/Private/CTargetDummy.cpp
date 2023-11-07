// Copyright (C) 2023 - Calvin Davidson

#include "CTargetDummy.h"
#include "CAttributeComponent.h"
#include "CCommonDefines.h"

// Sets default values
ACTargetDummy::ACTargetDummy()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");

	SetRootComponent(MeshComp);

	MeshComp->SetCollisionObjectType(COLLISION_ENEMY);
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
	AttributeComp->OnPlayerHealthChangedDelegate.AddDynamic(this, &ACTargetDummy::OnHealthChangedResponse);
}

void ACTargetDummy::OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponent* OwningComp, float Delta,
											float NewHealth)
{
	if (NewHealth <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TargetDummy destroyed"));
		Destroy();
	}
}

float ACTargetDummy::GetHealthPercent()
{
	return AttributeComp->GetHealthPercent();
}
