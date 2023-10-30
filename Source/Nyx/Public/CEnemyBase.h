// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CEnemyBase.generated.h"

class UStaticMeshComponent;
class UCAttributeComponent;

/*
 * Basic enemy class
 */
UCLASS()
class NYX_API ACEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCAttributeComponent> AttributeComp;

	UFUNCTION()
	void OnHealthChangedResponse(float Delta, float NewHealth);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();
};
