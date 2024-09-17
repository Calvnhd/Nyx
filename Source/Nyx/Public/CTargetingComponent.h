// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CTargetingComponent.generated.h"

class UCameraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCTargetingComponent();

protected:

	UPROPERTY(BlueprintReadOnly)
	AActor* TargetLockedActor;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Player|Abilities")
	float FindTargetTraceRadius;

	UPROPERTY()
	UCameraComponent* Camera;
	UPROPERTY()
	AActor* Muzzle;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//						   FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeReferences(UCameraComponent* InCamera, AActor* InMuzzle);

	FTransform GetCrosshairTargetTM() const;
	FTransform GetLockedTargetTM() const;
	FVector GetCameraTargetLocation() const;
	AActor* GetCameraTargetActor() const;

	void ClearTargetLockedActor();
	bool HasTargetLockedActor() const;
	bool FindNewTargetActor();
};
