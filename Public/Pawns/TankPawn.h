// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/BasePawn.h"
#include "Types.h"
#include "TankPawn.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATankPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//  Functions

public:

protected:
	virtual void Move(const FInputActionValue& Value);
	virtual void Look(const FInputActionValue& Value);
	void Accelerate();
	void Decelerate();


private:

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Movement")
	float MovementSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn Behavior | Movement")
	float TargetMovementSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Pawn Behavior | Movement")
	float RotationSpeed = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UAudioMovementDetection> AudioComponent;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AccelerateAction;

};
