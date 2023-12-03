// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAttributeComponent.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnPlayerHealthChangedSignature, AActor*, InstigatorActor,
											  UCAttributeComponent*, OwningComp, float, Delta, float, NewHealth);

/*
 * Contains and manages the player's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponent();

	// Static functions can be called anywhere, without an instance of the class
	UFUNCTION(BlueprintCallable, Category = "Nyx-Attributes")
	static UCAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx-Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	float GetSpeed();
	float GetThrust();
	void IncrementThrust();
	void DecrementThrust();
	bool IsAlive();
	// todo, add an instigator here
	void ApplyHealthChange(float Delta);
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetHealthMax();
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnPlayerHealthChangedSignature OnPlayerHealthChangedDelegate;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float ThrustPercent;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float ThrustPercentMax;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float SpeedMax;
};
