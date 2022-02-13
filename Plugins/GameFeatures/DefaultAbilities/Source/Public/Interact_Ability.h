#pragma once

#include "CoreMinimal.h"
#include "GAS/System/GASGameplayAbility.h"
#include "Interact_Ability.generated.h"

/**
 *
 */
UCLASS(MinimalAPI, NotPlaceable, HideDropdown, Category = "Custom GAS | Abilities")
class UInteract_Ability : public UGASGameplayAbility
{
	GENERATED_BODY()

public:
	UInteract_Ability(const FObjectInitializer& ObjectInitializer);

private:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	                             const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
