// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CAsteroidAttributeComponent.generated.h"

// Dynamic allows BP assignment, Multicast allows multiple listeners
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, AActor*, InstigatorActor,
//											  UCAttributeComponentBase*, OwningComp, float, Delta, float, NewHealth);

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

public:
	void InitializeAttributes();
	void TakeDamage(float Damage);
	bool IsAlive() const;

};
