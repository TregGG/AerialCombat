// Fill out your copyright notice in the Description page of Project Settings.


#include "AAircraftBase.h"

// Sets default values
AAAircraftBase::AAAircraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAAircraftBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAAircraftBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

