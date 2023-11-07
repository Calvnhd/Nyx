// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CEnemyBase.generated.h"

class UStaticMeshComponent;
class UCEnemyAttributeComponent;

/*
 * Basic enemy class
 */
UCLASS()
class NYX_API ACEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACEnemyBase();

	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCEnemyAttributeComponent> EnemyAttributeComp;

	UFUNCTION()
	void OnHealthChangedResponse(float Delta, float NewHealth);

	UFUNCTION()
	void OnCollisionResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							 FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();
};
