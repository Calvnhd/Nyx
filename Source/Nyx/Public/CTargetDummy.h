// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"

#include "CTargetDummy.generated.h"

class UStaticMeshComponent;
class UCAttributeComponent;

/*
 * A target dummy, for figuring out enemy stuff
 */
UCLASS()
class NYX_API ACTargetDummy : public APawn
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
	TObjectPtr<UCAttributeComponent> AttributeComp;

	UFUNCTION()
	void OnHealthChangedResponse(float Delta, float NewHealth);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();
};
