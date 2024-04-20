// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TurretPawn.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"

ATurretPawn::ATurretPawn() : Super()
{
}

void ATurretPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATurretPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		
	}
}
