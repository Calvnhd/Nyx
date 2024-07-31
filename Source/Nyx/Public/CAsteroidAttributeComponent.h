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
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	TEnumAsByte<EAsteroidSize> Size = EAsteroidSize::Base;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	int32 SizeMultiplier = 1;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	int32 BaseHealth = 100;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	int32 BasePower = 100;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	int32 Health = 100;
	UPROPERTY(VisibleAnywhere, Category = "Nyx|Attributes")
	int32 Power = 100;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	void CalculateAttributes(EAsteroidSize InSize);


};
