#pragma once

#include "CoreMinimal.h"
#include "EnvConfigs.generated.h"



USTRUCT(BlueprintType)
struct FEnvAirflow
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WindDirection = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindForce = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurbulenceStrength = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UpdraftForce = 0.f;
};