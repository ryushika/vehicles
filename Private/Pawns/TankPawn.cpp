// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/TankPawn.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "../Public/Components/AudioMovementDetection.h"

ATankPawn::ATankPawn() : Super()
{
	AudioComponent = CreateDefaultSubobject<UAudioMovementDetection>(TEXT("AudioComponent"));
	this->AddOwnedComponent(AudioComponent);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovementSpeed = FMath::FInterpTo(MovementSpeed, TargetMovementSpeed, DeltaTime, 20.0f);
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATankPawn::Move);

		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Started, this, &ATankPawn::Accelerate);
		EnhancedInputComponent->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &ATankPawn::Decelerate);
	}
}

void ATankPawn::Move(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (InputVector.Y >= 0)
	{
		RotationSpeed = FMath::Abs(RotationSpeed);
	}
	else
	{
		RotationSpeed = -FMath::Abs(RotationSpeed);
	}
	FVector ForwardDirection = GetActorForwardVector() * InputVector.Y * MovementSpeed * DeltaTime;
	FRotator Rotation = FRotator(0.f, InputVector.X * DeltaTime * RotationSpeed, 0.f);

	if (Controller)
	{
		AddActorWorldOffset(ForwardDirection, true);
		AddActorLocalRotation(Rotation, true);
	}
}

void ATankPawn::Look(const FInputActionValue& Value)
{
	Super::Look(Value);
}

void ATankPawn::Accelerate()
{
	TargetMovementSpeed = 750.0f;
	UE_LOG(LogTemp, Warning, TEXT("Accelerate"));
}

void ATankPawn::Decelerate()
{
	TargetMovementSpeed = 300.0f;
	UE_LOG(LogTemp, Warning, TEXT("Decelerate"));
}
