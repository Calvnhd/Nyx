// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CAttributeComponentBase.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAsteroidAttributeComponent.generated.h"

UENUM()
enum EAsteroidSize
{
	Base = 1,
	Small = 2,
	Medium = 5,
	Large = 10,
	Largest = 20
};

UENUM()
enum EAsteroidState
{
	Dormant,
	Active,
	Aggressive,
	Stunned
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAsteroidAttributeComponent : public UCAttributeComponentBase
{
	GENERATED_BODY()

public:
	UCAsteroidAttributeComponent();

	/* Getters */

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	EAsteroidSize GetSize() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	EAsteroidState GetState() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetDamagePower() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetScaledPhysicalPower();
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	uint8 GetNumberOfAsteroidsToSpawn() const;

	float GetChanceToSpawnItem() const;
	float GetAttributeModifier() const;

	/* Setters */

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Setters")
	void ModifyAttributes(float NewModifier = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Setters")
	void SetNumberOfAsteroidsToSpawn(uint8 Num);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Setters")
	void SetState(EAsteroidState NewState);

	void InitializeAttributes();

	/* Behaviour */

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Behaviour")
	bool TrySpawnItem() const;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	TEnumAsByte<EAsteroidSize> Size;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	TEnumAsByte<EAsteroidState> State;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float SizeMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float AttributeModifier;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Nyx|AttributeComponent|Attributes")
	float BaseHealth;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BaseDamagePower;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BasePhysicalPower;

	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float DamagePower;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Nyx|AttributeComponent|Attributes")
	float TimeToActivate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Nyx|AttributeComponent|Attributes")
	uint8 NumberOfAsteroidsToSpawn;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float ChanceToSpawnItem;
};
