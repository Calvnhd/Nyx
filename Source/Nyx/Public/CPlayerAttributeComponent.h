// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "CAttributeComponentBase.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CPlayerAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillPointsChangedSignature, UCAttributeComponentBase*, OwningComp,
											   float, Delta, float, NewPoints);

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

	UPROPERTY(BlueprintAssignable, Category = "Nyx|AttributeComponent|Events")
	FOnSkillPointsChangedSignature OnSkillPointsChanged;

protected:
	float SkillPoints;
	bool bIsInvulnerable;
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|AttributeComponent|Boost")
	float CurrentBoost;
	float MaxBoost;
	float BoostToDash;
	float BoostToJump;
	float BoostRecoveryIncrement;
	float BoostRecoveryRate;

	float BoostRecoveryCooldownTime;
	FTimerHandle BoostRecoveryCooldownTimerHandle;
	FTimerHandle BoostRecoveryTimerHandle;

	void RecoverBoost();
};
