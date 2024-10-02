// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CAttributeComponentBase.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CPlayerAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillPointsChangedSignature, UCAttributeComponentBase*, OwningComp,
											   float, Delta, float, NewPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBoostChangedSignature, UCPlayerAttributeComponent*, OwningComp, float, NewBoost);

/*
 * Contains and manages the player's attributes
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class NYX_API UCPlayerAttributeComponent : public UCAttributeComponentBase
{
	GENERATED_BODY()

public:
	UCPlayerAttributeComponent();

	void AddSkillPoints(float PointsToAdd);
	float GetSkillPoints() const;

	float ConsumeDashBoost();
	float ConsumeJumpBoost();

	UFUNCTION(BlueprintCallable, Category = "Nyx|AttributeComponent|Getters")
	float GetBoostPercent() const;

	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Events")
	FOnSkillPointsChangedSignature OnSkillPointsChanged;
	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Events")
	FOnBoostChangedSignature OnBoostChanged;

protected:
	float SkillPoints;
	bool bIsInvulnerable;
	float CurrentBoost;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float MaxBoost;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BoostToDash;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BoostToJump;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BoostRecoveryIncrement;
	UPROPERTY(EditDefaultsOnly, Category = "Nyx|AttributeComponent|Attributes")
	float BoostRecoveryRate;

	float BoostRecoveryCooldownTime;
	FTimerHandle BoostRecoveryCooldownTimerHandle;
	FTimerHandle BoostRecoveryTimerHandle;

	void RecoverBoost();

	void UpdateBoost(float Delta);
};
