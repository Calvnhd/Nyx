// Copyright (C) 2024 - Calvin Davidson

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
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	static UCAttributeComponentBase* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	bool IsAlive() const;
	void ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Attributes")
	FOnHealthChangedSignature OnHealthChanged;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetHealthMax() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetHealthPercent() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float HealthMax;
};
