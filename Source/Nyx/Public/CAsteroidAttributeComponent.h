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

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	static UCAsteroidAttributeComponent* GetAttributes(AActor* FromActor);
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsAsteroidAlive(AActor* Actor);

	void InitializeAttributes();
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetHealthPercent() const;
	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Attributes")
	float GetPower() const;

	EAsteroidSize GetSize() const;
	uint8 GetNumberOfAsteroidsToSpawn() const;
	TSubclassOf<AActor> GetAsteroidClassToSpawn() const;

	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Attributes")
	FOnHealthChangedDelegate OnHealthChanged;

	void ApplyHealthChange(AActor* InstigatorActor, float Delta);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	TEnumAsByte<EAsteroidSize> Size;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Nyx|AttributeComponent|Attributes")
	float BaseHealth;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Nyx|AttributeComponent|Attributes")
	float BasePower;

	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float SizeMultiplier;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float Health;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float Power;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Attributes")
	float HealthMax;

	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> AsteroidClass_Base;
	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> AsteroidClass_Small;
	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> AsteroidClass_Medium;
	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> AsteroidClass_Large;
	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> AsteroidClass_Largest;
	UPROPERTY(EditAnywhere, Category = "Nyx|AttributeComponent|Classes")
	TSubclassOf<AActor> ItemDropClass;


};
