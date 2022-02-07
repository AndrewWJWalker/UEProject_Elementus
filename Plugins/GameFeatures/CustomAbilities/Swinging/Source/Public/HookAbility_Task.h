#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "HookAbility_Task.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, NotBlueprintable, NotPlaceable, Category = "Custom GAS | Tasks")
class UHookAbility_Task : public UAbilityTask
{
	GENERATED_BODY()

public:
	UHookAbility_Task(const FObjectInitializer& ObjectInitializer);

	/* Create a reference to manage this ability task */
	static UHookAbility_Task* HookAbilityMovement(UGameplayAbility* OwningAbility,
	                                              FName TaskInstanceName,
	                                              const FHitResult HitResult);

	virtual void Activate() override;

private:
	virtual void TickTask(float DeltaTime) override;

	virtual void OnDestroy(bool AbilityIsEnding) override;

protected:
	bool bIsFinished;

	FHitResult HitDataHandle;
};
