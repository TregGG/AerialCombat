// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilityMasterComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class MYPROJECT_API UGameplayAbilityMasterComponent : public UGameplayAbility
{
	GENERATED_BODY()
public :
	UGameplayAbilityMasterComponent();

	//virtual void BeginPlay() override;
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ModifyAbilityBar(float Delta);
	float GetAbilityBarFill() const;

	// Input triggers (server-side)

	/*UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UseShoot();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UseAbilityPrimary();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UseAbilitySecondary();

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_UseAbilityTertiary();*/
	virtual void OnShoot();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float RateOfFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float ClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float MaxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float ShootRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	float ReloadTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shooting")
	USceneComponent* FirePoint;

	UPROPERTY(Replicated, EditAnywhere, Category = "Ability|Bar")
	float AbilityBarFill;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Bar")
	float MaxBarFill = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Backlash")
	float BacklashStartDelay = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Backlash")
	float BacklashTickInterval = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Backlash")
	float BacklashDamagePerTick = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Ability|Passive")
	float PassiveEffectScale = 1.0f;

	// -------- OVERRIDABLE HOOKS -------- //
	virtual void OnReload();
	virtual void OnAbilityPrimary();
	virtual void OnAbilitySecondary();
	virtual void OnAbilityTertiary();

	virtual void OnPassiveEffectTick(float NormalizedBar); // 0.0 - 1.0
	virtual void OnBacklashTick(float NormalizedBar); // 0.0 - 1.0

	UFUNCTION(BlueprintPure, Category = "Ability")
	FORCEINLINE float GetNormalizedBar() const { return AbilityBarFill / MaxBarFill; }

	// GAS Activation function - override this in child classes
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	float TimeSinceLastShoot = 0.0f;
	float TimeSinceLastAbilityUse = 0.0f;
	float BacklashTickTimer = 0.0f;

	void HandleBacklash(float DeltaTime);
	void HandlePassiveScaling();
};
