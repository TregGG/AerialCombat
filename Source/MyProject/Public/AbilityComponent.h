#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilityComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UAbilityComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAbilityComponent();

	virtual void BeginPlay() override;

	// Ability bar management (kept for compatibility, but now handled by AbilityManager)
	void ModifyAbilityBar(float Delta);
	float GetAbilityBarFill() const;

	UFUNCTION(BlueprintPure, Category = "Ability")
	FORCEINLINE float GetNormalizedBar() const { return AbilityBarFill / MaxBarFill; }

	//// GAS method wrappers for convenience
	//UFUNCTION(BlueprintCallable, Category = "Ability System")
	//FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& AbilitySpec);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



protected:
	// Ability bar properties (kept for compatibility)
	UPROPERTY(Replicated, EditAnywhere, Category = "Ability|Bar")
	float AbilityBarFill = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Bar")
	float MaxBarFill = 100.0f;
};
