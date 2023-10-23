// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"

#include "CTargetDummy.generated.h"

class UStaticMeshComponent;

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
};
