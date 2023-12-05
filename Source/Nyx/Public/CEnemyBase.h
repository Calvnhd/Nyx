// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CEnemyBase.generated.h"

class UCEnemyAttributeComponent;
// There is also AIPerception.  This is the older and simpler of the two.
class UPawnSensingComponent;
class UUserWidget;
class UCWorldUserWidget;

/*
 * Basic enemy class
 */
UCLASS()
class NYX_API ACEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACEnemyBase();

protected:

	UCWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, Category = "Nyx-UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "Nyx-Effects")
	FName TimeToHitParamName;

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Nyx-Components")
	TObjectPtr<UCEnemyAttributeComponent> EnemyAttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Nyx-Components")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UFUNCTION()
	void OnHealthChangedResponse(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta,
								 float NewHealth);

	UFUNCTION()
	void OnCollisionResponse(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
							 FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnPawnSeenResponse(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	void SetTargetActor(AActor* NewTarget);

public:
	bool IsAlive();
};
