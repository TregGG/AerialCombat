#include "AbilityComponent.h"
#include "Net/UnrealNetwork.h"
//#include "../ADroneBase.h"  
UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	/*if (AADroneBase* Drone = Cast<AADroneBase>(GetOwner()))
	{
		FirePoint = Drone->GetFirePoint();
	}*/
}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastAbilityUse += DeltaTime;
	BacklashTickTimer += DeltaTime;
	TimeSinceLastShoot += DeltaTime;

	HandleBacklash(DeltaTime);
	HandlePassiveScaling();

}

void UAbilityComponent::ModifyAbilityBar(float Delta)
{
	AbilityBarFill = FMath::Clamp(AbilityBarFill + Delta, 0.0f, MaxBarFill);
	TimeSinceLastAbilityUse = 0.0f;
	BacklashTickTimer = 0.0f;

}

float UAbilityComponent::GetAbilityBarFill() const
{
	return AbilityBarFill;
}

void UAbilityComponent::Server_UseShoot_Implementation()
{
	if (TimeSinceLastShoot >= 0 && MaxAmmo>0 && ClipAmmo>0) {
		OnShoot();
		TimeSinceLastShoot = -RateOfFire;
		MaxAmmo -= 1;
		ClipAmmo -= 1;
	}
	else if (TimeSinceLastShoot >= 0 && MaxAmmo > 0 && ClipAmmo <= 0) {
		OnReload();
	}
}

void UAbilityComponent::Server_UseAbilityPrimary_Implementation()
{
	OnAbilityPrimary();
	TimeSinceLastAbilityUse = 0.0f;
}

void UAbilityComponent::Server_UseAbilitySecondary_Implementation()
{
	OnAbilitySecondary();
	TimeSinceLastAbilityUse = 0.0f;
}

void UAbilityComponent::Server_UseAbilityTertiary_Implementation()
{
	OnAbilityTertiary();
	TimeSinceLastAbilityUse = 0.0f;
}

void UAbilityComponent::HandleBacklash(float DeltaTime)
{
	if (TimeSinceLastAbilityUse >= BacklashStartDelay && BacklashTickTimer >= BacklashTickInterval)
	{
		OnBacklashTick(GetNormalizedBar());
		BacklashTickTimer = 0.0f;
	}
}

void UAbilityComponent::HandlePassiveScaling()
{
	OnPassiveEffectTick(GetNormalizedBar());
}

void UAbilityComponent::OnShoot()
{
	
		//if (!FirePoint) return;

		AActor* Owner = GetOwner();
		if (!Owner) return;

		APawn* PawnOwner = Cast<APawn>(Owner);
		if (!PawnOwner) return;

		AController* Controller = PawnOwner->GetController();
		if (!Controller) return;
	

		// Get FirePoint location
		//const FVector Start = FirePoint->GetComponentLocation();
		const FVector Start = PawnOwner->GetTargetLocation();

		// Get Pitch from camera
		FVector CamLoc;
		FRotator CamRot;
		Controller->GetPlayerViewPoint(CamLoc, CamRot);
		const float Pitch = CamRot.Pitch;

		// Get Yaw from actor
		const float Yaw = Owner->GetActorRotation().Yaw;

		// Combine
		const FRotator ShootRot = FRotator(Pitch, Yaw, 0.0f);
		const FVector End = Start + ShootRot.Vector() * ShootRange;

		// Raycast
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Owner);

		const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

		if (bHit)
		{
			//UGameplayStatics::ApplyDamage(Hit.GetActor(), ShootDamage, Controller, Owner, nullptr);

			
		}

		

		DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 1.5f, 0, 1.0f);
	

}

void UAbilityComponent::OnReload()
{
	TimeSinceLastShoot = -ReloadTime;
	ClipAmmo = MaxClipAmmo;
}

// Default no-ops for override
void UAbilityComponent::OnAbilityPrimary() {}
void UAbilityComponent::OnAbilitySecondary() {}
void UAbilityComponent::OnAbilityTertiary() {}
void UAbilityComponent::OnPassiveEffectTick(float NormalizedBar) {}
void UAbilityComponent::OnBacklashTick(float NormalizedBar) {}

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAbilityComponent, AbilityBarFill);
}
