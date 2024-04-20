// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TurretCrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTurretCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()
	
//  Functions

public:
	void InitiateReload(float ReloadTime);

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void Reloading(float ReloadTime);

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class URoundProgressBar* StaticCircle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class URoundProgressBar* ReloadCircle;

private:
	UPROPERTY()
	bool bIsReloading = false;

	UPROPERTY()
	float TickReloadTime = 0.f;

	UPROPERTY()
	float CurrentReloadTime = 0.f;
};
