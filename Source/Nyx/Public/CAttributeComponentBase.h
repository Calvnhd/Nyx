// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAttributeComponentBase.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatorActor,
											  UCAttributeComponentBase*, OwningComp, float, Delta, float, NewHealth);

/*
 * Base class for storing and managing attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAttributeComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponentBase();

	// Static functions can be called anywhere, without an instance of the class
	UFUNCTION(BlueprintCallable, Category = "Nyx|Attributes")
	static UCAttributeComponentBase* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);

	bool IsAlive();
	void ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetHealthMax();
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent();

	UPROPERTY(BlueprintAssignable, Category = "Nyx|Attributes")
	FOnHealthChangedSignature OnHealthChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Attributes")
	float HealthMax;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Attributes")
	float ThrustPercent;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Attributes")
	float ThrustPercentMax;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|Attributes")
	float SpeedMax;
};
