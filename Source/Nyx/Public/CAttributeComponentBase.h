// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAttributeComponentBase.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatorActor, UCAttributeComponentBase*, OwningComp, float,
											  Delta, float, NewHealth);

/*
 * Base class for storing and managing attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAttributeComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UCAttributeComponentBase();

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	static UCAttributeComponentBase* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Setters")
	virtual void ApplyHealthChange(AActor* InstigatorActor, float Delta);
	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetHealthMax() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetHealthPercent() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float HealthMax;
};
