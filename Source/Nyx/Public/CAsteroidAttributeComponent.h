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

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCAsteroidAttributeComponent : public UCAttributeComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCAsteroidAttributeComponent();

	void InitializeAttributes();
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	void ModifyAttributes(float NewModifier = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	EAsteroidSize GetSize() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetDamagePower() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetScaledPhysicalPower();
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	uint8 GetNumberOfAsteroidsToSpawn() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	void SetNumberOfAsteroidsToSpawn(uint8 Num);
	float GetSpawnItemChance() const;
	float GetAttributeModifier() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	bool TrySpawnItem() const;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	TEnumAsByte<EAsteroidSize> Size;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float SizeMultiplier;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float AttributeModifier;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BaseHealth;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BaseDamagePower;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BasePhysicalPower;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float DamagePower;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float TimeToActivate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	uint8 NumberOfAsteroidsToSpawn;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float SpawnItemChance;
};
