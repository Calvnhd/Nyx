// Copyright (C) 2024 - Calvin Davidson

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
	virtual void PostInitializeComponents() override;

	// The class we'll use for the pop up health bar, to be assigned in BP
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	// Actual reference to the pop up health bar for use in code
	UCWorldUserWidget* ActiveHealthBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nyx|Effects")
	FName TimeToHitParamName;

	UPROPERTY(VisibleAnywhere, Category = "Nyx|Components")
	TObjectPtr<UCEnemyAttributeComponent> EnemyAttributeComp;

	UPROPERTY(VisibleAnywhere, Category = "Nyx|Components|AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UFUNCTION()
	void HealthChangedHandler(AActor* InstigatorActor, UCAttributeComponentBase* OwningComp, float Delta, float NewHealth);

	UFUNCTION()
	void CollisionHandler(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						  const FHitResult& Hit);

	UFUNCTION()
	void PawnSeenHandler(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	void SetTargetActor(AActor* NewTarget);

public:
	bool IsAlive();
};
