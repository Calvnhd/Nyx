// Copyright (C) 2023 - Calvin Davidson

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "PAttributeComponentBase.generated.h"

// Dynamic allows BP assignment
// Multicast allows multiple listeners
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Delta);

/*
 * Contains and manages the common attributes for a character
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROTO_API UPAttributeComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPAttributeComponentBase();

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float Health;
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	float HealthMax;
	// UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	// float HealthQuanta;

public:
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//						   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(float Damage);

	// UPROPERTY(BlueprintAssignable, Category = "Attributes")
	// FOnHealthChangedSignature OnHealthChangedTrigger;
};
