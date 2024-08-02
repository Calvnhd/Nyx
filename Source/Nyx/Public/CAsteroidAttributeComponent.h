// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAsteroidAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedDelegate, AActor*, InstigatorActor,
											  UCAsteroidAttributeComponent*, OwningComp, float, Delta, float,
											  NewHealth);

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
class NYX_API UCAsteroidAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCAsteroidAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Nyx|Attributes")
	static UCAsteroidAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsAsteroidAlive(AActor* Actor);

	void InitializeAttributes();
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable)
	float GetPower() const;

	EAsteroidSize GetSize() const;

	UPROPERTY(BlueprintAssignable, Category = "Nyx|Attributes")
	FOnHealthChangedDelegate OnHealthChanged;

	void ApplyHealthChange(AActor* InstigatorActor, float Delta);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|Attributes")
	TEnumAsByte<EAsteroidSize> Size;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Nyx|Attributes")
	float BaseHealth;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Nyx|Attributes")
	float BasePower;

	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Attributes")
	float SizeMultiplier;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Attributes")
	float Health;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Attributes")
	float Power;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|Attributes")
	float HealthMax;
};
