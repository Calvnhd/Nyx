// Copyright (C) 2024 - Calvin Davidson

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "CWorldUserWidget.generated.h"

class USizeBox;

/**
 *
 */
UCLASS()
class NYX_API UCWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Something we can move across the screen
	// BindWidget lets the pointer be filled as long as we create something in BP with a matching name
	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// UPROPERTY is also good for reference counting. Will take care of cleaning up the pointer and stuff for us.
	UPROPERTY(BlueprintReadOnly, Category = "Nyx|UI")
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, Category = "Nyx|UI")
	FVector WorldOffset;
};
