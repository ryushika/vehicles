// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AudioMovementDetection.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UAudioMovementDetection : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAudioMovementDetection();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//  Functions
public:
	
protected:

private:
	
	//  Variables
public:
	FVector LastFramePosition;
	float CurrentRawSpeed;
	
	float CurrentSmoothSpeed;
	float SpeedSumForAverage;
	TArray<float> SpeedHistory;
	int SpeedHistoryLength;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Speed")
	float SmoothedSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Speed")
	float MaxSpeed;
	
protected:

private:
	
};
