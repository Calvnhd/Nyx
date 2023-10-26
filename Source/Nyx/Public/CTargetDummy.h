// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "CAttributeInterface.h"

#include "CTargetDummy.generated.h"

class UStaticMeshComponent;
class UCEnemyAttributeComponent;

/*
 * A target dummy, for figuring out enemy stuff
 */
UCLASS()
class NYX_API ACTargetDummy : public APawn, public ICAttributeInterface
{
	GENERATED_BODY()

public:
	ACTargetDummy();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCEnemyAttributeComponent> AttributeComp;

	// Attribute Interface Implementation
	// Unreal expects _Implementation suffix because this is a BlueprintNativeEvent in ICAttributeInterface
	virtual void ApplyDamage_Implementation(APawn* AttackingPawn, float DamageAmount) override;
};
