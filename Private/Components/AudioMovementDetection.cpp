#include "Components/AudioMovementDetection.h"

UAudioMovementDetection::UAudioMovementDetection()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentRawSpeed = 0;
	CurrentSmoothSpeed = 0;
	SpeedSumForAverage = 0;
	SpeedHistoryLength = 10;
	MaxSpeed = 600;
}

void UAudioMovementDetection::BeginPlay()
{
	Super::BeginPlay();
	LastFramePosition = GetOwner()->GetActorLocation();
}

void UAudioMovementDetection::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector CurrentPosition = GetOwner()->GetActorLocation();
	CurrentRawSpeed = FVector::Dist(CurrentPosition, LastFramePosition) / DeltaTime;

	SpeedSumForAverage+= CurrentRawSpeed;
	SpeedHistory.Add(CurrentRawSpeed);

	if (SpeedHistory.Num() > SpeedHistoryLength)
	{
		SpeedSumForAverage -= SpeedHistory[0];
		SpeedHistory.RemoveAt(0);
	}

	CurrentSmoothSpeed = SpeedSumForAverage / FMath::Min(SpeedHistoryLength, SpeedHistory.Num());
	SmoothedSpeed = FMath::Clamp(CurrentSmoothSpeed / MaxSpeed, 0.0f, 1.0f);

	LastFramePosition = CurrentPosition;
}

