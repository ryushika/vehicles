// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BasePawn.h"
#include "TurretPawn.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATurretPawn : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATurretPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//  Functions

public:

protected:

private:

//  Variables

public:

protected:

private:
};
