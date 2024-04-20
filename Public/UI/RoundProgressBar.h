// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundProgressBar.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API URoundProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
//  Functions

public:
	UFUNCTION(BlueprintCallable, Category = "Progress Bar")
	void SetPercent(float Percent);

protected:

private:

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* ProgressBarImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
	class UMaterialInstanceDynamic* ProgressBarInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress Bar")
	class UMaterialInterface* ProgressBarMaterial;

private:
};
