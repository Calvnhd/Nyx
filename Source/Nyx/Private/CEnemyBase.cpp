// Copyright (C) 2023 - Calvin Davidson

#include "CEnemyBase.h"
#include "CAttributeComponent.h"
#include "CCommonDefines.h"

// Sets default values
ACEnemyBase::ACEnemyBase()
{
	//PrimaryActorTick.bCanEverTick = true;
	AttributeComp = CreateDefaultSubobject<UCAttributeComponent>("AttributeComp");
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
//void ACEnemyBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}
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
